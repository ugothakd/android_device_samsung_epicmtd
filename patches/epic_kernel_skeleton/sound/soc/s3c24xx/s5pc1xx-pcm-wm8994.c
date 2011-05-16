/*
 * smdkc110_wm8580.c
 *
 * Copyright (C) 2009, Samsung Elect. Ltd. - Jaswinder Singh <jassisinghbrar@gmail.com>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 */

#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/initval.h>
#include <asm/io.h>
#include <asm/gpio.h> 
#include <plat/gpio-cfg.h> 
#include <plat/map-base.h>
#include <mach/regs-clock.h>
#include <linux/delay.h>

#include "../codecs/wm8994.h"
#include "s3c-dma.h"
#include "s3c-pcmdev.h"

#define PLAY_51       0
#define PLAY_STEREO   1
#define PLAY_OFF      2

#define REC_MIC    0
#define REC_LINE   1
#define REC_OFF    2

#define SRC_CLK	s3c_i2s_get_clockrate()


#define CONFIG_SND_DEBUG
#ifdef CONFIG_SND_DEBUG
#define debug_msg(x...) printk(x)
#else
#define debug_msg(x...)
#endif
extern struct s5p_pcm_pdata s3c_pcm_pdat;
static int smdkc110_play_opt;
static int smdkc110_rec_opt;

static int smdkc110_set_pcm_clk(int id)
{
        switch(id)
        {
        case 0:
                writel(((readl(S5P_CLK_SRC6)&~S5P_CLKSRC6_AUDIO0_MASK)|(0x1<<S5P_CLKSRC6_AUDIO0_SHIFT)),S5P_CLK_SRC6);
                //writel(readl(S5P_SCLKGATE0)|S5P_CLKGATE_SCLK0_AUDIO0,S5P_SCLKGATE0);
                break;
        case 1:
                writel(((readl(S5P_CLK_SRC6)&~S5P_CLKSRC6_AUDIO1_MASK)|(0x1<<S5P_CLKSRC6_AUDIO1_SHIFT)),S5P_CLK_SRC6);
                //writel(readl(S5P_SCLKGATE0)|S5P_CLKGATE_SCLK0_AUDIO1,S5P_SCLKGATE0);
                break;
        case 2:
                writel(((readl(S5P_CLK_SRC6)&~S5P_CLKSRC6_AUDIO2_MASK)|(0x1<<S5P_CLKSRC6_AUDIO2_SHIFT)),S5P_CLK_SRC6);
                //writel(readl(S5P_SCLKGATE0)|S5P_CLKGATE_SCLK0_AUDIO2,S5P_SCLKGATE0);
                break;
        default:
                debug_msg("Not a valid PCM IP Number. - %d\n", id);
                return -EINVAL;
        }
        return 0;
}

static int smdkc110_set_gpio(int id)
{
        switch(id)
        {
        case 0:
                s3c_gpio_cfgpin (S5PV210_GPC1(0), S3C_GPIO_SFN(3));
                s3c_gpio_cfgpin (S5PV210_GPC1(1), S3C_GPIO_SFN(3));
                s3c_gpio_cfgpin (S5PV210_GPC1(2), S3C_GPIO_SFN(3));
                s3c_gpio_cfgpin (S5PV210_GPC1(3), S3C_GPIO_SFN(3));
                s3c_gpio_cfgpin (S5PV210_GPC1(4), S3C_GPIO_SFN(3));

                s3c_gpio_setpull(S5PV210_GPC1(0), S3C_GPIO_PULL_NONE);
                s3c_gpio_setpull(S5PV210_GPC1(1), S3C_GPIO_PULL_NONE);
                s3c_gpio_setpull(S5PV210_GPC1(2), S3C_GPIO_PULL_NONE);
                s3c_gpio_setpull(S5PV210_GPC1(3), S3C_GPIO_PULL_NONE);
                s3c_gpio_setpull(S5PV210_GPC1(4), S3C_GPIO_PULL_NONE);
                
		break;
        case 1:
                s3c_gpio_cfgpin (S5PV210_GPC0(0), S3C_GPIO_SFN(3));
                s3c_gpio_cfgpin (S5PV210_GPC0(1), S3C_GPIO_SFN(3));
                s3c_gpio_cfgpin (S5PV210_GPC0(2), S3C_GPIO_SFN(3));
                s3c_gpio_cfgpin (S5PV210_GPC0(3), S3C_GPIO_SFN(3));
                s3c_gpio_cfgpin (S5PV210_GPC0(4), S3C_GPIO_SFN(3));
		
		s3c_gpio_setpull(S5PV210_GPC0(0), S3C_GPIO_PULL_NONE);
                s3c_gpio_setpull(S5PV210_GPC0(1), S3C_GPIO_PULL_NONE);
                s3c_gpio_setpull(S5PV210_GPC0(2), S3C_GPIO_PULL_NONE);
                s3c_gpio_setpull(S5PV210_GPC0(3), S3C_GPIO_PULL_NONE);
                s3c_gpio_setpull(S5PV210_GPC0(4), S3C_GPIO_PULL_NONE);
		break;

	case 2:
                s3c_gpio_cfgpin (S5PV210_GPI(0), S3C_GPIO_SFN(3));
                s3c_gpio_cfgpin (S5PV210_GPI(1), S3C_GPIO_SFN(3));
                s3c_gpio_cfgpin (S5PV210_GPI(2), S3C_GPIO_SFN(3));
                s3c_gpio_cfgpin (S5PV210_GPI(3), S3C_GPIO_SFN(3));
                s3c_gpio_cfgpin (S5PV210_GPI(4), S3C_GPIO_SFN(3));

                s3c_gpio_setpull(S5PV210_GPI(0), S3C_GPIO_PULL_NONE);
                s3c_gpio_setpull(S5PV210_GPI(1), S3C_GPIO_PULL_NONE);
                s3c_gpio_setpull(S5PV210_GPI(2), S3C_GPIO_PULL_NONE);
                s3c_gpio_setpull(S5PV210_GPI(3), S3C_GPIO_PULL_NONE);
                s3c_gpio_setpull(S5PV210_GPI(4), S3C_GPIO_PULL_NONE);
		
		break;
        default:
                debug_msg("Not a valid PCM IP Number. - %d\n", id);
                return -EINVAL;
        }
        return 0;
}
       
/* XXX BLC(bits-per-channel) --> BFS(bit clock shud be >= FS*(Bit-per-channel)*2) XXX */
/* XXX BFS --> RFS(must be a multiple of BFS)                                 XXX */
/* XXX RFS & SRC_CLK --> Prescalar Value(SRC_CLK / RFS_VAL / fs - 1)          XXX */
static int smdkc110_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params)
{
	u32 epll_vsel, epll_m, epll_p, epll_s, epll_div,value;
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct snd_soc_dai *cpu_dai = rtd->dai->cpu_dai;
	struct snd_soc_dai *codec_dai = rtd->dai->codec_dai;
	int bfs, rfs, psr, ret;

	debug_msg("+++++++++%s\n", __FUNCTION__);
	//TODO need to check below function in detail     
         //smdkc110_set_pcm_clk(PCM_ID);

//All below calls to configure CPU_DAI has to be implemented properly ..

	/* Set the AP DAI pcm mode configuration */
        ret = snd_soc_dai_set_fmt(cpu_dai, SND_SOC_DAIFMT_DSP_A);
        if (ret < 0)
                return ret;
	//..these values kept as it was in smdkc110 spdif driver
        /* The values of epll_vsel, epll_m, epll_p, epll_s and epll_div are taken from firmware code. */
	switch (params_rate(params)) {
                case 32000:
                        epll_vsel = 1;
                        epll_m = 131;
                        epll_p = 6;
                        epll_s = 4;
                        epll_div = 1;
                        break;


		case 44100:
                        epll_vsel = 0;
                        epll_m = 271;
                        epll_p = 18;
                        epll_s = 3;
                        epll_div = 1;
                        break;
                case 48000:
                        epll_vsel = 0;
                        epll_m = 131;
                        epll_p = 8;
                        epll_s = 3;
                        epll_div = 1;
                        break;
		case 96000:
                        epll_vsel = 0;
                        epll_m = 131;
                        epll_p = 8;
                        epll_s = 3;
                        epll_div = 0;
                        break;
                default:
                        epll_vsel = 0;
                        epll_m = 271;
                        epll_p = 18;
                        epll_s = 3;
                        break;
        }

	/* Clock Settings */
        // Mask output clock of AUDIO1
         writel(readl(S5P_CLK_SRC_MASK0) |(0x1<<25)/*S5P_CLKSRC_MASK0_AUDIO1*/, S5P_CLK_SRC_MASK0);


         /* MUX EPLL */
         writel(((readl(S5P_CLK_SRC0) & ~S5P_CLKSRC0_EPLL_MASK) | (0x1 << S5P_CLKSRC0_EPLL_SHIFT)), S5P_CLK_SRC0);

	/* Enable clock gating of AUDIO1  */
        //value = readl(S5P_CLKGATE_SCLK0);
        //value |= S5P_CLKGATE_SCLK0_AUDIO1;
        //writel(value, S5P_CLKGATE_SCLK0);

        /* SCLK_AUDIO1, SCLK_EPLL */
        value = readl(S5P_CLK_SRC6);
        value &= ~ (S5P_CLKSRC6_AUDIO1_MASK);
        value |= (0x7<<4);//EPLL clock selection
        writel(value, S5P_CLK_SRC6);

	/* EPLL Lock Time, 300us */
        writel(((readl(S5P_EPLL_LOCK) &  ~(0xFFFF<<0)) | (0xE10<<0)), S5P_EPLL_LOCK);

        /* EPLL control setting */
        value = readl(S5P_EPLL_CON);
        value &= !(S5P_EPLL_MASK_VSEL | S5P_EPLL_MASK_M | S5P_EPLL_MASK_P | S5P_EPLL_MASK_S);
        value |= S5P_EPLL_EN | epll_vsel << 27 | epll_m << 16 | epll_p << 8 | epll_s << 0;
      //  writel(value, S5P_EPLL_CON); 

        /* Wait till EPLL is locked */
        /* Not sure if such while loop is good - system may hang */
        //while(!(readl(S5P_EPLL_CON)>>29)&0x1);
        

	/* Set clock divider */
    //    writel(((readl(S5P_CLK_DIV6) & ~(0xF<<4)) | (epll_div<<4)), S5P_CLK_DIV6);// commented




	/* Set the Codec DAI configuration */
//	ret = snd_soc_dai_set_fmt(codec_dai, SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFS);
//	if (ret < 0)
//		return ret;

	debug_msg("----------%s\n", __FUNCTION__);
	return 0;
}

/*
 * WM8580 DAI operations.
 */
static struct snd_soc_ops smdkc110_ops = {
	.hw_params = smdkc110_hw_params,
};

static void smdkc110_ext_control(struct snd_soc_codec *codec)
{
	debug_msg("%s\n", __FUNCTION__);

	/* set up jack connection */
	if(smdkc110_play_opt == PLAY_51){
		snd_soc_dapm_enable_pin(codec, "Front-L/R");
		snd_soc_dapm_enable_pin(codec, "Center/Sub");
		snd_soc_dapm_enable_pin(codec, "Rear-L/R");
	}else if(smdkc110_play_opt == PLAY_STEREO){
		snd_soc_dapm_enable_pin(codec, "Front-L/R");
		snd_soc_dapm_disable_pin(codec, "Center/Sub");
		snd_soc_dapm_disable_pin(codec, "Rear-L/R");
	}else{
		snd_soc_dapm_disable_pin(codec, "Front-L/R");
		snd_soc_dapm_disable_pin(codec, "Center/Sub");
		snd_soc_dapm_disable_pin(codec, "Rear-L/R");
	}

	if(smdkc110_rec_opt == REC_MIC){
		snd_soc_dapm_enable_pin(codec, "MicIn");
		snd_soc_dapm_disable_pin(codec, "LineIn");
	}else if(smdkc110_rec_opt == REC_LINE){
		snd_soc_dapm_disable_pin(codec, "MicIn");
		snd_soc_dapm_enable_pin(codec, "LineIn");
	}else{
		snd_soc_dapm_disable_pin(codec, "MicIn");
		snd_soc_dapm_disable_pin(codec, "LineIn");
	}

	/* signal a DAPM event */
	snd_soc_dapm_sync(codec);
}

static int smdkc110_get_pt(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	debug_msg("%s\n", __FUNCTION__);

	ucontrol->value.integer.value[0] = smdkc110_play_opt;
	return 0;
}

static int smdkc110_set_pt(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	debug_msg("%s\n", __FUNCTION__);

	if(smdkc110_play_opt == ucontrol->value.integer.value[0])
		return 0;

	smdkc110_play_opt = ucontrol->value.integer.value[0];
	smdkc110_ext_control(codec);
	return 1;
}

static int smdkc110_get_cs(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	debug_msg("%s\n", __FUNCTION__);
	ucontrol->value.integer.value[0] = smdkc110_rec_opt;
	return 0;
}

static int smdkc110_set_cs(struct snd_kcontrol *kcontrol,
	struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec =  snd_kcontrol_chip(kcontrol);
	debug_msg("%s\n", __FUNCTION__);

	if(smdkc110_rec_opt == ucontrol->value.integer.value[0])
		return 0;

	smdkc110_rec_opt = ucontrol->value.integer.value[0];
	smdkc110_ext_control(codec);
	return 1;
}
static int smdkc110_wm8994_init(struct snd_soc_codec *codec)
{
	return 0;
}

extern struct snd_soc_dai wm8994_pcm_dai;
static struct snd_soc_dai_link smdkc110_dai[] = {
{
	.name = "PCM-C110",
	.stream_name = "PCM FOR VT",
	.cpu_dai = &s3c_pcmdev_dai,
	.codec_dai = &wm8994_pcm_dai,
	.init = smdkc110_wm8994_init,
	.ops = &smdkc110_ops,
},
};

static struct snd_soc_card smdkc110 = {
	.name = "smdkc110-pcm",
	.platform = &s3c24xx_pcm_soc_platform,
	.dai_link = smdkc110_dai,
	.num_links = ARRAY_SIZE(smdkc110_dai),
};

static struct wm8994_setup_data smdkc110_wm8994_setup = {
	.i2c_address = 0x34>>1,
	.i2c_bus = 4,
};

extern struct snd_soc_codec_device soc_codec_dev_pcm_wm8994;

static struct snd_soc_device smdkc110_snd_devdata = {
	.card = &smdkc110,
	.codec_dev = &soc_codec_dev_pcm_wm8994,
	.codec_data = &smdkc110_wm8994_setup,
};

static struct platform_device *smdkc110_snd_device;
static struct snd_soc_device *socdev = &smdkc110_snd_devdata ;
static int __init smdkc110_audio_init(void)
{
	int ret;

	printk(KERN_ERR "chk %s:s5pc1xx-pcm-wm8994.c \n", __FUNCTION__);
	debug_msg("+++++++%s\n", __FUNCTION__);
	
	if(smdkc110_set_gpio(PCM_ID)){
		 debug_msg("++++++++%s..@%d\n", __FUNCTION__,__LINE__);
		 return -EINVAL;
	}

	smdkc110_snd_device = platform_device_alloc("soc-audio", 1);
	if (!smdkc110_snd_device){
		 debug_msg("++++++++%s..@%d\n", __FUNCTION__,__LINE__);
		return -ENOMEM;
	}	

	platform_set_drvdata(smdkc110_snd_device, &smdkc110_snd_devdata);
	smdkc110_snd_devdata.dev = &smdkc110_snd_device->dev;
	ret = platform_device_add(smdkc110_snd_device);

	printk(KERN_ERR "chk %s: ret %d \n", __FUNCTION__, ret);
	if (ret){
		debug_msg("###going to put platform device ..%s\n", __FUNCTION__);
		printk(KERN_ERR "chkerr %s : going to put platform device \n", __FUNCTION__);
		platform_device_put(smdkc110_snd_device);
	}

	debug_msg("----------%s\n", __FUNCTION__);
	
	return ret;
}


static void __exit smdkc110_audio_exit(void)
{
	debug_msg("%s\n", __FUNCTION__);

	platform_device_unregister(smdkc110_snd_device);
}

module_init(smdkc110_audio_init);
module_exit(smdkc110_audio_exit);

/* Module information */
MODULE_DESCRIPTION("ALSA SoC SMDKC110 ");
MODULE_LICENSE("GPL");
