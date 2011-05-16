/*
 * s5pc1xx_wm8994.c
 *
 * Copyright (C) 2010, Samsung Elect. Ltd. - 
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 */

#include <linux/platform_device.h>
#include <linux/clk.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <mach/regs-clock.h>
#include <plat/regs-iis.h>
#include "../codecs/wm8994.h"
#include "s3c-dma.h"
#include "s3c64xx-i2s.h"
#include "s3c-i2s-v2.h"

#include <linux/io.h>

#define I2S_NUM 0

extern struct snd_soc_dai i2s_sec_fifo_dai;
extern struct snd_soc_dai i2s_dai;
extern struct snd_soc_platform s3c_dma_wrapper;
extern const struct snd_kcontrol_new s5p_idma_control;

//static int set_epll_rate(unsigned long rate); //not-used


//#define SRC_CLK	(i2s->clk_rate) 
#define SRC_CLK 66738000 	 

//#define CONFIG_SND_DEBUG
#ifdef CONFIG_SND_DEBUG
#define debug_msg(x...) printk(x)
#else
#define debug_msg(x...)
#endif

/*  BLC(bits-per-channel) --> BFS(bit clock shud be >= FS*(Bit-per-channel)*2)  */
/*  BFS --> RFS(must be a multiple of BFS)                                  */
/*  RFS & SRC_CLK --> Prescalar Value(SRC_CLK / RFS_VAL / fs - 1)           */
int smdkc110_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->dai->cpu_dai;
	struct snd_soc_dai *codec_dai = rtd->dai->codec_dai;
	int bfs, rfs, ret;
	u32 ap_codec_clk;
	debug_msg("%s\n", __FUNCTION__);

	/* Choose BFS and RFS values combination that is supported by
	 * both the WM8994 codec as well as the S5P AP
	 *
	 */
	switch (params_format(params)) {
		case SNDRV_PCM_FORMAT_S8:
			bfs = 16;
			rfs = 256;		/* Can take any RFS value for AP */
	 		break;
	 	case SNDRV_PCM_FORMAT_S16_LE:
			bfs = 32;
			rfs = 256;		/* Can take any RFS value for AP */
	 		break;
		case SNDRV_PCM_FORMAT_S20_3LE:
	 	case SNDRV_PCM_FORMAT_S24_LE:
			bfs = 48;
			rfs = 512;		/* B'coz 48-BFS needs atleast 512-RFS acc to *S5P6440* UserManual */
	 		break;
	 	case SNDRV_PCM_FORMAT_S32_LE:	/* Impossible, as the AP doesn't support 64fs or more BFS */
		default:
			return -EINVAL;
 	}

	/* Set the Codec DAI configuration */
	ret = snd_soc_dai_set_fmt(codec_dai, SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBM_CFM);
	
	if (ret < 0)
	{
		printk(KERN_ERR "smdkc110_wm8994_hw_params : Codec DAI configuration error!\n");
		return ret;
	}

	/* Set the AP DAI configuration */
	ret = snd_soc_dai_set_fmt(cpu_dai, SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBM_CFM);
        
	if (ret < 0)
	{
		printk(KERN_ERR "smdkc110_wm8994_hw_params : AP DAI configuration error!\n");
		return ret;
	}

	/* Select the AP Sysclk */
	ret = snd_soc_dai_set_sysclk(cpu_dai, S3C64XX_CDCLKSRC_EXT, params_rate(params), SND_SOC_CLOCK_IN);

	if (ret < 0)
	{
		printk("smdkc110_wm8994_hw_params : AP sys clock INT setting error!\n");
		return ret;
	}

        ret = snd_soc_dai_set_sysclk(cpu_dai, S3C64XX_CLKSRC_I2SEXT, params_rate(params), SND_SOC_CLOCK_IN);
        if (ret < 0)
	{
		printk("smdkc110_wm8994_hw_params : AP sys clock I2SEXT setting error!\n");
                return ret;
	}

       switch (params_rate(params)) {
                
		case 8000:
                        ap_codec_clk = 4096000; //Shaju ..need to figure out this rate
		break;

                case 11025:
                        ap_codec_clk = 2822400; 
		break;
                case 12000:
                        ap_codec_clk = 6144000; 
		break;

                case 16000:
                        ap_codec_clk = 4096000;
		break;

                case 22050:
                        ap_codec_clk = 6144000;
		break;
                case 24000:
                        ap_codec_clk = 6144000;
		break;

                case 32000:
                        ap_codec_clk = 8192000;
		break;
                case 44100:
                        ap_codec_clk = 11289600;
		break;

                case 48000:
                        ap_codec_clk = 12288000;
		break;

                default:
                        ap_codec_clk = 11289600;
                        break;
                }

        ret = snd_soc_dai_set_sysclk(codec_dai, WM8994_SYSCLK_FLL, ap_codec_clk, 0);
        if (ret < 0)
                return ret;
	return 0;

}


static int smdkc110_wm8994_init(struct snd_soc_codec *codec)
{
	return 0;
}


/* machine stream operations */
static struct snd_soc_ops smdkc110_ops = {
	.hw_params = smdkc110_hw_params,
};

/* digital audio interface glue - connects codec <--> CPU */
static struct snd_soc_dai_link smdkc1xx_dai = {
//{
	.name = "WM8994",
	.stream_name = "WM8994 HiFi Playback",
//	.cpu_dai = &i2s_dai,
	.cpu_dai = &s3c64xx_i2s_dai[I2S_NUM],
	.codec_dai = &wm8994_dai,
	.init = smdkc110_wm8994_init,
	.ops = &smdkc110_ops,
//},
};

static struct snd_soc_card smdkc100 = {
	.name = "smdkc110",
	.platform = &s3c_dma_wrapper,
	.dai_link = &smdkc1xx_dai,
	.num_links = 1,//ARRAY_SIZE(smdkc1xx_dai),
};

static struct wm8994_setup_data smdkc110_wm8994_setup = {
	/*
		The I2C address of the WM89940 is 0x34. To the I2C driver 
		the address is a 7-bit number hence the right shift .
	*/
	.i2c_address = 0x34,
	.i2c_bus = 4,
};

/* audio subsystem */
static struct snd_soc_device smdkc1xx_snd_devdata = {
	.card = &smdkc100,
	.codec_dev = &soc_codec_dev_wm8994,
	.codec_data = &smdkc110_wm8994_setup,
};

static struct platform_device *smdkc1xx_snd_device; 
static int __init smdkc110_audio_init(void)
{
	int ret;

	printk(KERN_ERR "chk %s: aries-wm8994.c \n", __FUNCTION__);
	debug_msg("%s\n", __FUNCTION__);

	smdkc1xx_snd_device = platform_device_alloc("soc-audio", 0);
	if (!smdkc1xx_snd_device)
		return -ENOMEM;
        printk(KERN_DEBUG "chk %s: aries-wm8994.c platform device allocate \n", __FUNCTION__);
	platform_set_drvdata(smdkc1xx_snd_device, &smdkc1xx_snd_devdata);
	smdkc1xx_snd_devdata.dev = &smdkc1xx_snd_device->dev;
	ret = platform_device_add(smdkc1xx_snd_device);
        printk(KERN_ERR "chk %s: aries-wm8994.c ret value is %d \n", __FUNCTION__,ret);
	if (ret)	{
		printk(KERN_ERR "chkerr %s : going to put platform device \n", __FUNCTION__);
		platform_device_put(smdkc1xx_snd_device);
	}

	return ret;
}

static void __exit smdkc110_audio_exit(void)
{
	debug_msg("%s\n", __FUNCTION__);

	platform_device_unregister(smdkc1xx_snd_device);
}

module_init(smdkc110_audio_init);
module_exit(smdkc110_audio_exit);

/* Module information */
MODULE_DESCRIPTION("ALSA SoC SMDKC110 WM8994");
MODULE_LICENSE("GPL");
