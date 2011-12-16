Team EpicCM Device Repo
=======================

How to build
------------
This is a quick tutorial on building an ota package for Samsung Epic using the CyanogenMod Android repo.

This tutorial begins with a belief that you have already sync'd down CyanogenMod sources and have set up your build environment.

* First, create a `local_manifest.xml` for the EpicCM device and to override cyanogen vendor.

```xml
<?xml version="1.0" encoding="UTF-8"?>
  <manifest>
    <remote name="EpicCM" fetch="https://github.com" />

    <project
      path="device/samsung/epicmtd"
      name="EpicCM/android_device_samsung_epicmtd"
      remote="EpicCM" revision="blob-overlay"
     />
    <project
      path="vendor/cyanogen"
      name="EpicCM/android_vendor_cyanogen"
      remote="EpicCM"
      revision="epiccm-rebase"
    />
  </manifest>
```

* To set up the vendor tree, you'll need to `cd` to `device/samsung/epicmtd` and:

  1. Run `extract-files.sh` while phone is connected via usb to pull proprietaries from device. The script will then call `setup-makefiles` to generate `vendor/samsung/epicmtd/` and the necessary makefiles for blob manipulation.

  2. Next, from the root android directory, you'll need to:

```
repo sync
make clobber
make brunch epicmtd
```

and the build will begin!

At the end of the compile, the generate ota package will be in `out/target/product/epicmtd/`
