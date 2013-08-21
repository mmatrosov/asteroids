LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := asteroids

LOCAL_CPPFLAGS := -std=c++11

LOCAL_CPP_FEATURES += exceptions
LOCAL_CPP_FEATURES += rtti

LOCAL_SRC_FILES := \
    exports.cpp \
    Application.cpp \
    Shapes.cpp \
    geometry.cpp \

LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog

$(LOCAL_LDLIBS) += libsupc++.a

include $(BUILD_SHARED_LIBRARY)
