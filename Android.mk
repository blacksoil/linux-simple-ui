LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES := graphics.c events.c
LOCAL_MODULE := libblacksoil-minui
LOCAL_MODULE_TAGS := optional
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := main.c
LOCAL_MODULE := blacksoil-exec
LOCAL_MODULE_TAGS := optional
LOCAL_STATIC_LIBRARIES := libblacksoil-minui libcutils
LOCAL_SHARED_LIBRARIES := libpixelflinger
include $(BUILD_EXECUTABLE)

#include $(CLEAR_VARS)
#LOCAL_SRC_FILES := input-tester.c
#LOCAL_MODULE := blacksoil-input-tester
#local_module_TAGS := optional
#LOCAL_STATIC LIBRARIES := libblacksoil-minui libcutils
#include $(BUILD_EXECUTABLE)
