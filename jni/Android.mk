LOCAL_PATH := $(call my-dir)
###############################################################################
#       audioHelper
#       author:zd
###############################################################################
include $(CLEAR_VARS)
LOCAL_MODULE            :=  ffmpegHelper
LOCAL_SRC_FILES         :=  ffmpegHelper/com_ring0_ffmpeg_FFmpegHelper.cpp \
                            ffmpegHelper/yuv/simple_yuv.cpp
LOCAL_C_INCLUDES        :=  $(LOCAL_PATH)/ffmpeg-3.3.4/android/$(TARGET_ARCH_ABI)/include \
                            $(LOCAL_PATH)/ffmpegHelper/yuv
LOCAL_LDLIBS            +=  -L$(SYSROOT)/usr/lib -llog -lm -lz -lGLESv2 -Wl,-s
LOCAL_CFLAGS            +=  -fvisibility=hidden
LOCAL_SHARED_LIBRARIES  :=  ffmpeg
include $(BUILD_SHARED_LIBRARY)
###############################################################################
#       openglHelper
#       author:zd
###############################################################################
include $(CLEAR_VARS)
LOCAL_MODULE            :=  openglHelper
LOCAL_SRC_FILES         :=  openglHelper/com_ring0_opengl_OpenGLHelper.cpp
LOCAL_ARM_MODE          :=  arm
LOCAL_CFLAGS            +=  -fvisibility=hidden
LOCAL_LDLIBS            :=  -llog -landroid -lm -lz -lGLESv2
include $(BUILD_SHARED_LIBRARY)
###############################################################################
#       openslHelper
#       author:zd
###############################################################################
include $(CLEAR_VARS)
LOCAL_MODULE            :=  openslHelper
LOCAL_SRC_FILES         :=  openslHelper/com_ring0_opensl_OpenSLHelper.cpp
LOCAL_ARM_MODE          :=  arm
LOCAL_CFLAGS            +=  -fvisibility=hidden
LOCAL_LDLIBS            :=  -llog -landroid -lm -lz -lOpenSLES
include $(BUILD_SHARED_LIBRARY)
###############################################################################
#       ffmpeg
#       author:zd
###############################################################################
include $(CLEAR_VARS)
LOCAL_MODULE            :=  ffmpeg
LOCAL_SRC_FILES         :=  ffmpeg-3.3.4/android/$(TARGET_ARCH_ABI)/libffmpeg.so
LOCAL_LDLIBS            +=  -L$(SYSROOT)/usr/lib -llog
include $(PREBUILT_SHARED_LIBRARY)
