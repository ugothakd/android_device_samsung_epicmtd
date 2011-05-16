/*
 *  linux/arch/arm/kernel/time.c
 *
 *  Copyright (C) 1991, 1992, 1995  Linus Torvalds
 *  Modifications for ARM (C) 1994-2001 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  This file contains the ARM-specific time handling details:
 *  reading the RTC at bootup, etc...
 *
 *  1994-07-02  Alan Modra
 *              fixed set_rtc_mmss, fixed time.year for >= 2000, new mktime
 *  1998-12-20  Updated NTP code according to technical memorandum Jan '96
 *              "A Kernel Model for Precision Timekeeping" by Dave Mills
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/timex.h>
#include <linux/errno.h>
#include <linux/profile.h>
#include <linux/sysdev.h>
#include <linux/timer.h>
#include <linux/irq.h>

#include <linux/mc146818rtc.h>

#include <asm/leds.h>
#include <asm/thread_info.h>
#include <asm/stacktrace.h>
#include <asm/mach/time.h>

/*
 * Our system timer.
 */
struct sys_timer *system_timer;

#if defined(CONFIG_RTC_DRV_CMOS) || defined(CONFIG_RTC_DRV_CMOS_MODULE)
/* this needs a better home */
DEFINE_SPINLOCK(rtc_lock);

#ifdef CONFIG_RTC_DRV_CMOS_MODULE
EXPORT_SYMBOL(rtc_lock);
#endif
#endif	/* pc-style 'CMOS' RTC support */

/* change this if you have some constant time drift */
#define USECS_PER_JIFFY	(1000000/HZ)

#ifdef CONFIG_SMP
unsigned long profile_pc(struct pt_regs *regs)
{
	struct stackframe frame;

	if (!in_lock_functions(regs->ARM_pc))
		return regs->ARM_pc;

	frame.fp = regs->ARM_fp;
	frame.sp = regs->ARM_sp;
	frame.lr = regs->ARM_lr;
	frame.pc = regs->ARM_pc;
	do {
		int ret = unwind_frame(&frame);
		if (ret < 0)
			return 0;
	} while (in_lock_functions(frame.pc));

	return frame.pc;
}
EXPORT_SYMBOL(profile_pc);
#endif

/*
 * hook for setting the RTC's idea of the current time.
 */
int (*set_rtc)(void);


static unsigned long next_rtc_update;

/*
 * If we have an externally synchronized linux clock, then update
 * CMOS clock accordingly every ~11 minutes.  set_rtc() has to be
 * called as close as possible to 500 ms before the new second
 * starts.
 */
static inline void do_set_rtc(void)
{
	if (!ntp_synced() || set_rtc == NULL)
		return;

	if (next_rtc_update &&
	    time_before((unsigned long)xtime.tv_sec, next_rtc_update))
		return;

	if (xtime.tv_nsec < 500000000 - ((unsigned) tick_nsec >> 1) &&
	    xtime.tv_nsec >= 500000000 + ((unsigned) tick_nsec >> 1))
		return;

	if (set_rtc())
		/*
		 * rtc update failed.  Try again in 60s
		 */
		next_rtc_update = xtime.tv_sec + 60;
	else
		next_rtc_update = xtime.tv_sec + 660;
}

#ifdef CONFIG_LEDS

static void dummy_leds_event(led_event_t evt)
{
}

void (*leds_event)(led_event_t) = dummy_leds_event;

struct leds_evt_name {
	const char	name[8];
	int		on;
	int		off;
};

static const struct leds_evt_name evt_names[] = {
	{ "amber", led_amber_on, led_amber_off },
	{ "blue",  led_blue_on,  led_blue_off  },
	{ "green", led_green_on, led_green_off },
	{ "red",   led_red_on,   led_red_off   },
};

static ssize_t leds_store(struct sys_device *dev,
			struct sysdev_attribute *attr,
			const char *buf, size_t size)
{
	int ret = -EINVAL, len = strcspn(buf, " ");

	if (len > 0 && buf[len] == '\0')
		len--;

	if (strncmp(buf, "claim", len) == 0) {
		leds_event(led_claim);
		ret = size;
	} else if (strncmp(buf, "release", len) == 0) {
		leds_event(led_release);
		ret = size;
	} else {
		int i;

		for (i = 0; i < ARRAY_SIZE(evt_names); i++) {
			if (strlen(evt_names[i].name) != len ||
			    strncmp(buf, evt_names[i].name, len) != 0)
				continue;
			if (strncmp(buf+len, " on", 3) == 0) {
				leds_event(evt_names[i].on);
				ret = size;
			} else if (strncmp(buf+len, " off", 4) == 0) {
				leds_event(evt_names[i].off);
				ret = size;
			}
			break;
		}
	}
	return ret;
}

static SYSDEV_ATTR(event, 0200, NULL, leds_store);

static int leds_suspend(struct sys_device *dev, pm_message_t state)
{
	leds_event(led_stop);
	return 0;
}

static int leds_resume(struct sys_device *dev)
{
	leds_event(led_start);
	return 0;
}

static int leds_shutdown(struct sys_device *dev)
{
	leds_event(led_halted);
	return 0;
}

static struct sysdev_class leds_sysclass = {
	.name		= "leds",
	.shutdown	= leds_shutdown,
	.suspend	= leds_suspend,
	.resume		= leds_resume,
};

static struct sys_device leds_device = {
	.id		= 0,
	.cls		= &leds_sysclass,
};

static int __init leds_init(void)
{
	int ret;
	ret = sysdev_class_register(&leds_sysclass);
	if (ret == 0)
		ret = sysdev_register(&leds_device);
	if (ret == 0)
		ret = sysdev_create_file(&leds_device, &attr_event);
	return ret;
}

device_initcall(leds_init);

EXPORT_SYMBOL(leds_event);
#endif

#ifdef CONFIG_LEDS_TIMER
static inline void do_leds(void)
{
	static unsigned int count = HZ/2;

	if (--count == 0) {
		count = HZ/2;
		leds_event(led_timer);
	}
}
#else
#define	do_leds()
#endif


/**
 * save_time_delta - Save the offset between system time and RTC time
 * @delta: pointer to timespec to store delta
 * @rtc: pointer to timespec for current RTC time
 *
 * Return a delta between the system time and the RTC time, such
 * that system time can be restored later with restore_time_delta()
 */
void save_time_delta(struct timespec *delta, struct timespec *rtc)
{
	set_normalized_timespec(delta,
				xtime.tv_sec - rtc->tv_sec,
				xtime.tv_nsec - rtc->tv_nsec);
}
EXPORT_SYMBOL(save_time_delta);

/**
 * restore_time_delta - Restore the current system time
 * @delta: delta returned by save_time_delta()
 * @rtc: pointer to timespec for current RTC time
 */
void restore_time_delta(struct timespec *delta, struct timespec *rtc)
{
	struct timespec ts;

	set_normalized_timespec(&ts,
				delta->tv_sec + rtc->tv_sec,
				delta->tv_nsec + rtc->tv_nsec);

	do_settimeofday(&ts);
}
EXPORT_SYMBOL(restore_time_delta);


#define timer_suspend NULL
#define timer_resume NULL

static struct sysdev_class timer_sysclass = {
	.name		= "timer",
	.suspend	= timer_suspend,
	.resume		= timer_resume,
};

static int __init timer_init_sysfs(void)
{
	int ret = sysdev_class_register(&timer_sysclass);
	if (ret == 0) {
		system_timer->dev.cls = &timer_sysclass;
		ret = sysdev_register(&system_timer->dev);
	}

	return ret;
}

device_initcall(timer_init_sysfs);

void __init time_init(void)
{
	system_timer->init();
}

