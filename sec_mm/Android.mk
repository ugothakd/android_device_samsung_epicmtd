USE_SEC_OMX := true

ifeq ($(USE_SEC_OMX), true)
  include $(all-subdir-makefiles)
endif
