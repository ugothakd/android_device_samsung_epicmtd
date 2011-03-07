# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# BoardConfig.mk
#
# Product-specific compile-time definitions.
#
# If the BoardConfigVendor.mk exists, inherit it.
-include vendor/samsung/epic/BoardConfigVendor.mk

TARGET_NO_BOOTLOADER := true
#TARGET_NO_RECOVERY := true
#TARGET_NO_KERNEL := true

TARGET_BOARD := SPH-D700
TARGET_BOARD_PLATFORM := s5pc110
TARGET_BOARD_PLATFORM_GPU := POWERVR_SGX540_120

# ARMv7-A Cortex-A8 architecture
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_ARCH_VARIANT := armv7-a-neon
TARGET_GLOBAL_CFLAGS += -mtune=cortex-a8
TARGET_GLOBAL_CPPFLAGS += -mtune=cortex-a8
ARCH_ARM_HAVE_TLS_REGISTER := true
ANDROID_ARM_LINKER := true

BOARD_USES_HGL := true

# Set Audio related defines below.
BOARD_USES_GENERIC_AUDIO := false

BOARD_NO_RGBX_8888 := true

# Set Camera related defines below.
#BOARD_USES_OVERLAY := false
#BOARD_USES_COPYBIT := true
#DEFAULT_FB_NUM := 0
BOARD_USES_FROYO_GRALLOC := true

BOARD_USE_FROYO_LIBCAMERA := true
BOARD_USE_REVERSE_FFC := true
USE_CAMERA_STUB := false
USE_OVERLAY_FORMAT_YCbCr_420_SP := true
BUILD_PV_VIDEO_ENCODERS := 1
BOARD_CAMERA_LIBRARIES := libcamera libcameraservice

BOARD_V4L2_DEVICE := /dev/video1
BOARD_CAMERA_DEVICE := /dev/video0
BOARD_SECOND_CAMERA_DEVICE := /dev/video2


# Set WIFI related defined below
BOARD_WPA_SUPPLICANT_DRIVER := WEXT
WPA_SUPPLICANT_VERSION := VER_0_6_X
WIFI_DRIVER_FW_STA_PATH := "/system/etc/wifi/bcm4329_sta.bin"
WIFI_DRIVER_FW_AP_PATH := "/system/etc/wifi/bcm4329_aps.bin"
BOARD_WLAN_DEVICE := bcm4329
WIFI_DRIVER_MODULE_PATH := "/lib/modules/dhd.ko"
WIFI_DRIVER_MODULE_ARG := "firmware_path=/system/etc/wifi/bcm4329_sta.bin nvram_path=/system/etc/wifi/nvram_net.txt dhd_watchdog_ms=10 dhd_poll=1"
WIFI_DRIVER_MODULE_NAME := "dhd"
#WIFI_IFACE_DIR := "/data/misc/wifi"
CONFIG_DRIVER_WEXT := true
BOARD_WEXT_NO_COMBO_SCAN := true

# Set USB Tether related defines below.
RNDIS_DEVICE := "/sys/devices/virtual/sec/switch/tethering"

# Set Bluetooth related defines below.
BOARD_HAVE_BLUETOOTH_BCM := true
BOARD_HAVE_BLUETOOTH := true
BT_USE_BTL_IF := true
BT_ALT_STACK := true
BRCM_BTL_INCLUDE_A2DP := true
BRCM_BT_USE_BTL_IF := true
WITH_A2DP := true

# EGL
BOARD_EGL_CFG := device/samsung/epic/prebuilt/egl.cfg

# Set Kernel and device related defines below.
USE_YAMAHA_SENSORS := true
TARGET_PREBUILT_KERNEL := device/samsung/epic/kernel
BOARD_KERNEL_CMDLINE := no_console_suspend=1 console=null
BOARD_KERNEL_BASE := 0x02e00000

BOARD_HAS_NO_SELECT_BUTTON := true
BOARD_HAS_NO_MISC_PARTITION := true
BOARD_USES_FFORMAT := true
BOARD_RECOVERY_IGNORE_BOOTABLES := true

BOARD_BOOTIMAGE_MAX_SIZE := $(call image-size-from-data-size,0x00280000)
BOARD_RECOVERYIMAGE_MAX_SIZE := $(call image-size-from-data-size,0x00500000)
BOARD_SYSTEMIMAGE_MAX_SIZE := $(call image-size-from-data-size,0x07500000)
BOARD_USERDATAIMAGE_MAX_SIZE := $(call image-size-from-data-size,0x04ac0000)
# The size of a block that can be marked bad.
BOARD_FLASH_BLOCK_SIZE := 131072

BOARD_USES_BMLUTILS := true

# Setup the release tools. This allows us to build OTA Packages.
TARGET_RELEASETOOLS_EXTENSIONS := device/samsung/epic

# Set ClockworkMod related defines below.
BOARD_UMS_LUNFILE := "/sys/devices/platform/s3c-usbgadget/gadget/lun0/file"
BOARD_CUSTOM_RECOVERY_KEYMAPPING := ../../device/samsung/epic/recovery/recovery_ui.c
