/* San Angeles Observation OpenGL ES version example
 * Copyright 2009 The Android Open Source Project
 * All rights reserved.
 *
 * This source is free software; you can redistribute it and/or
 * modify it under the terms of EITHER:
 *   (1) The GNU Lesser General Public License as published by the Free
 *     Software Foundation; either version 2.1 of the License, or (at
 *     your option) any later version. The text of the GNU Lesser
 *     General Public License is included with this source in the
 *     file LICENSE-LGPL.txt.
 *   (2) The BSD-style license that is included with this source in
 *     the file LICENSE-BSD.txt.
 *
 * This source is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files
 * LICENSE-LGPL.txt and LICENSE-BSD.txt for more details.
 */
#include <jni.h>
#include <memory>
#include "Globals.h"

#include "Application.h"

std::unique_ptr<CApplication> g_pTheApp;

extern "C" { 
  /* Call to initialize the graphics state */
  void Java_com_example_Asteroids_DemoRenderer_nativeInit(JNIEnv* env)
  {
    g_pTheApp.reset(new CApplication());
  }

  void Java_com_example_Asteroids_DemoRenderer_nativeResize(JNIEnv* env, jobject thiz, jint w, jint h )
  {
    g_pTheApp->OnResize(w, h);
  }

  /* Call to finalize the graphics state */
  void Java_com_example_Asteroids_DemoRenderer_nativeDone(JNIEnv* env)
  {
    g_pTheApp.reset();
  }

  void Java_com_example_Asteroids_DemoGLSurfaceView_nativeTouchEvent(JNIEnv* env, jobject thiz, jfloat x, jfloat y)
  {
    g_pTheApp->OnTouch(x, y);
  }

  void Java_com_example_Asteroids_DemoGLSurfaceView_nativePause(JNIEnv* env)
  {
  }

  void Java_com_example_Asteroids_DemoGLSurfaceView_nativeResume(JNIEnv* env)
  {
  }

  /* Call to render the next GL frame */
  void Java_com_example_Asteroids_DemoRenderer_nativeRender(JNIEnv* env)
  {
    g_pTheApp->Render();
  }
}

namespace Globals
{
  int ScreenWidth()
  {
    return g_pTheApp->GetScreenWidth();
  }

  int ScreenHeight()
  {
    return g_pTheApp->GetScreenHeight();
  }
}