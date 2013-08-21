#pragma once

#ifndef _MSC_VER
#  include <android/log.h>

#  define  LOG_TAG    "Asteroids"
#  define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#  define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__) 
#else
#  define  LOGI(...)
#  define  LOGE(...)
#endif