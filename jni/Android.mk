LOCAL_PATH := $(call my-dir)
###############################################################################
#       effect
#       author:zd
###############################################################################
include $(CLEAR_VARS)
LOCAL_MODULE            :=  effect
LOCAL_SRC_FILES         :=  effect/phase.cpp        \
                            effect/wahwah.cpp       \
                            effect/crystailzer.cpp  \
                            effect/stereo.cpp       \
                            effect/voiceremoval.cpp \
                            effect/echo.cpp
LOCAL_C_INCLUDES        :=  $(LOCAL_PATH)/effect
LOCAL_ARM_MODE          :=  arm
LOCAL_CFLAGS            +=  -fvisibility=hidden
LOCAL_LDLIBS            :=  -llog -lm -lz
include $(BUILD_SHARED_LIBRARY)
###############################################################################
#       ffmpegHelper
#       author:zd
###############################################################################
include $(CLEAR_VARS)
LOCAL_MODULE            :=  ffmpegHelper
LOCAL_SRC_FILES         :=  ffmpegHelper/com_ring0_ffmpeg_FFmpegHelper.cpp \
                            ffmpegHelper/yuv/simple_yuv.cpp \
                            ffmpegHelper/ffmpeg/ffmpeg.c \
                            ffmpegHelper/ffmpeg/ffmpeg_opt.c \
                            ffmpegHelper/ffmpeg/ffmpeg_filter.c \
                            ffmpegHelper/ffmpeg/cmdutils.c
LOCAL_C_INCLUDES        :=  $(LOCAL_PATH)/ffmpegHelper/ffmpeg                             \
                            $(LOCAL_PATH)/ffmpegHelper/yuv                                \
                            $(LOCAL_PATH)/ffmpeg-3.3.4/android/$(TARGET_ARCH_ABI)/include \
                            $(LOCAL_PATH)/ffmpeg-3.3.4
LOCAL_LDLIBS            +=  -L$(SYSROOT)/usr/lib -llog -lm -lz -lGLESv2 -landroid -lOpenSLES -Wl,-s
LOCAL_CFLAGS            +=  -fvisibility=hidden
LOCAL_ARM_MODE          :=  arm
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
#       x264
#       author:zd
###############################################################################
include $(CLEAR_VARS)
LOCAL_MODULE            :=  x264
LOCAL_SRC_FILES         :=  x264/android/$(TARGET_ARCH_ABI)/libx264.so
LOCAL_LDLIBS            +=  -L$(SYSROOT)/usr/lib -llog
include $(PREBUILT_SHARED_LIBRARY)
###############################################################################
#       ffmpeg
#       author:zd
###############################################################################
include $(CLEAR_VARS)
LOCAL_MODULE            :=  ffmpeg
LOCAL_SRC_FILES         :=  ffmpeg-3.3.4/android/$(TARGET_ARCH_ABI)/libffmpeg.so
LOCAL_LDLIBS            +=  -L$(SYSROOT)/usr/lib -llog
LOCAL_SHARED_LIBRARIES  :=  x264
include $(PREBUILT_SHARED_LIBRARY)
