#pragma once

// This is not actually a precompiled header, just a convenient place to put all the 
// includes together

// Includes for NDK compiler
#ifndef _MSC_VER

#include <GLES/gl.h>
#include <time.h>

#include <android/log.h>

#define  LOG_TAG "Asteroids"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__) 

#else
// Includes for Visual Studio

#define NOMINMAX
#include <Windows.h>
#include <gl/GL.h>

#define  LOGI(...)
#define  LOGE(...)

#endif

// Common includes
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <algorithm>
#include <random>
#include <cmath>
#include <iterator>

// Stuff
const float PI = 3.14159265358979323846f;