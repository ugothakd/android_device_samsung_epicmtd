ifeq ($(TARGET_DEVICE),epic)
WITH_SEC_OMX := true

ifeq ($(WITH_SEC_OMX), true)
  include $(all-subdir-makefiles)
endif
endif
