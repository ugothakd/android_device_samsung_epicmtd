#
# Copyright (C) 2011 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

$(call inherit-product, $(SRC_TARGET_DIR)/product/small_base.mk)

# The gps config for this device
$(call inherit-product, device/common/gps/gps_us_supl.mk)

# Include the smdkc110.rc file
PRODUCT_COPY_FILES += \
    device/samsung/epic/init.smdkc110.rc:root/init.smdkc110.rc

## Property Overrides
PRODUCT_PROPERTY_OVERRIDES += \
    ro.sf.lcd_density=240 \
    rild.libpath=/system/lib/libsec-ril40.so \
    rild.libargs=-d /dev/ttyS0 \
    wifi.interface=eth0 \
    wifi.supplicant_scan_interval=90 \
    ro.wifi.channels=11 \
    ro.telephony.default_network=4 \
    ro.com.google.clientidbase=android-sprint-us \
    ro.cdma.home.operator.numeric=310120 \
    ro.cdma.home.operator.alpha=Sprint \
    net.cdma.pppd.authtype=require-pap \
    net.cdma.pppd.user=user SprintNextel \
    net.cdma.datalinkinterface=/dev/ttyCDMA0 \
    net.interfaces.defaultroute=cdma \
    net.cdma.ppp.interface=ppp0 \
    net.connectivity.type=CDMA1 \
    ro.csc.sales_code=SPR \
    ril.sales_code=SPR \
    ro.carrier=Sprint \
    net.dns1=8.8.8.8 \
    net.dns2=8.8.4.4 \
    media.stagefright.enable-player=true \
    media.stagefright.enable-meta=true \
    media.stagefright.enable-scan=true \
    media.stagefright.enable-http=true \
    dalvik.vm.startheapsize=8m \
    dalvik.vm.heapsize=32m \
    mobiledata.interfaces=eth0,ppp0 \
    ro.opengles.version=131072

# we have enough storage space to hold precise GC data
PRODUCT_TAGS += dalvik.gc.type-precise
    
# Use high-density artwork when available
PRODUCT_LOCALES := hdpi

DEVICE_PACKAGE_OVERLAYS += device/samsung/epic/overlay

# media profiles and capabilities spec
$(call inherit-product, device/samsung/epic/media_a1026.mk)

# Libs
PRODUCT_PACKAGES += \
    libSEC_OMX_Core \
    libOMX.SEC.AVC.Decoder \
    libOMX.SEC.M4V.Decoder \
    libOMX.SEC.M4V.Encoder \
    libOMX.SEC.AVC.Encoder \
    libaudio \
    libaudiopolicy \
    sec_mm \    
    libstagefrighthw \
    overlay.s5pc110 \
    copybit.s5pc110 \
    lights.s5pc110 \
    libcamera
        
# media config xml file
PRODUCT_COPY_FILES += \
    device/samsung/epic/media_profiles.xml:system/etc/media_profiles.xml

# asound.conf
PRODUCT_COPY_FILES += \
    device/samsung/epic/prebuilt/asound.conf:system/etc/asound.conf
	
PRODUCT_COPY_FILES += \
    device/samsung/epic/prebuilt/apns-conf.xml:system/etc/apns-conf.xml \
    device/samsung/epic/prebuilt/glgps_samsungJupiter:system/bin/gpsd/glgps_samsungJupiter \
    device/samsung/epic/prebuilt/jupiter.xml:system/etc/jupiter.xml \
    device/samsung/epic/prebuilt/libgps.so:system/lib/libgps.so \
    device/samsung/epic/prebuilt/libgps.so:obj/lib/libgps.so \
    device/samsung/epic/prebuilt/libsecgps.so:system/lib/libsecgps.so \
    device/samsung/epic/prebuilt/libsecgps.so:obj/lib/libsecgps.so \
    device/samsung/epic/prebuilt/wpa_supplicant.conf:system/etc/wifi/wpa_supplicant.conf \
    device/samsung/epic/sec_mm/sec_omx/sec_omx_core/secomxregistry:system/etc/secomxregistry
    
# Install the features available on this device.
PRODUCT_COPY_FILES += \
    frameworks/base/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml \
    frameworks/base/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml \
    frameworks/base/data/etc/android.hardware.telephony.cdma.xml:system/etc/permissions/android.hardware.telephony.cdma.xml \
    frameworks/base/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/base/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/base/data/etc/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml \
    frameworks/base/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
    frameworks/base/data/etc/android.hardware.touchscreen.multitouch.distinct.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.distinct.xml \
    frameworks/base/data/etc/android.hardware.sensor.gyroscope.xml:system/etc/permissions/android.hardware.sensor.gyroscope.xml \
    frameworks/base/data/etc/android.software.sip.voip.xml:system/etc/permissions/android.software.sip.voip.xml \
    packages/wallpapers/LivePicker/android.software.live_wallpaper.xml:system/etc/permissions/android.software.live_wallpaper.xml
 
# Keylayout and Keychars
PRODUCT_COPY_FILES += \
     device/samsung/epic/prebuilt/keylayout/s3c-keypad.kl:system/usr/keylayout/s3c-keypad.kl \
     device/samsung/epic/prebuilt/keylayout/sec_jack.kl:system/usr/keylayout/sec_jack.kl \
     device/samsung/epic/prebuilt/keylayout/AVRCP.kl:system/usr/keylayout/AVRCP.kl \
     device/samsung/epic/prebuilt/keylayout/qt602240_ts_input.kl:system/usr/keylayout/qt602240_ts_input.kl \
     device/samsung/epic/prebuilt/keylayout/melfas-touchkey.kl:system/usr/keylayout/melfas-touchkey.kl \
     device/samsung/epic/prebuilt/keylayout/qwerty.kl:system/usr/keylayout/qwerty.kl \
     device/samsung/epic/prebuilt/keychars/s3c-keypad.kcm.bin:system/usr/keychars/s3c-keypad.kcm.bin \
     device/samsung/epic/prebuilt/keychars/sec_jack.kcm.bin:system/usr/keychars/sec_jack.kcm.bin \
     device/samsung/epic/prebuilt/keychars/melfas-touchkey.kcm.bin:system/usr/keychars/melfas-touchkey.kcm.bin \
     device/samsung/epic/prebuilt/keychars/qwerty.kcm.bin:system/usr/keychars/qwerty.kcm.bin \
     device/samsung/epic/prebuilt/keychars/qt602240_ts_input.kcm.bin:system/usr/keychars/qt602240_ts_input.kcm.bin \
     device/samsung/epic/prebuilt/keychars/qwerty2.kcm.bin:system/usr/keychars/qwerty2.kcm.bin  

# Vold configuration and fstab files
PRODUCT_COPY_FILES += \
    device/samsung/epic/prebuilt/vold.conf:system/etc/vold.conf \
    device/samsung/epic/prebuilt/vold.fstab:system/etc/vold.fstab
    
# We are using a prebuilt kernel for now, to ease building. This will be changed later.
ifeq ($(TARGET_PREBUILT_KERNEL),)
LOCAL_KERNEL := device/samsung/epic/kernel
else
LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
endif

PRODUCT_COPY_FILES += \
    $(LOCAL_KERNEL):kernel


PRODUCT_NAME := full_epic
PRODUCT_DEVICE := epic
PRODUCT_MODEL := SAMSUNG-SPH-D700
PRODUCT_BRAND := Samsung
PRODUCT_MANUFACTURER := Samsung

$(call inherit-product-if-exists, vendor/samsung/epic/epic-vendor.mk)
