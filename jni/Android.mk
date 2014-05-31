LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LS_CPP=$(subst $(1)/,,$(wildcard $(1)/*.cpp))
LOCAL_MODULE    := pacman
LOCAL_CFLAGS    := -Wno-psabi
LOCAL_SRC_FILES := $(call LS_CPP,$(LOCAL_PATH))
LOCAL_LDLIBS    := -landroid -llog -lEGL -lGLESv1_CM -lOpenSLES

LOCAL_STATIC_LIBRARIES := android_native_app_glue

LOCAL_LDLIBS += -L$(LOCAL_PATH)/../lib -lpng
LOCAL_LDLIBS += -lz
LOCAL_C_INCLUDES := $(LOCAL_PATH)/libh

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
