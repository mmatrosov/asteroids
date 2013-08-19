LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := sanangeles

LOCAL_CPPFLAGS := -std=c++11

LOCAL_CPP_FEATURES += exceptions

LOCAL_SRC_FILES := \
    exports.cpp \
    Application.cpp \

LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog

include $(BUILD_SHARED_LIBRARY)
