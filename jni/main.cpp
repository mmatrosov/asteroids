/* San Angeles Observation OpenGL ES version example
 * Copyright 2004-2005 Jetro Lauha
 * All rights reserved.
 * Web: http://iki.fi/jetro/
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
 *
 * $Id: demo.c,v 1.10 2005/02/08 20:54:39 tonic Exp $
 * $Revision: 1.10 $
 */

#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <assert.h>

//#include "importgl.h"
#include <GLES/gl.h>

#include "app.h"

#include <android/log.h>

float g_touchX = 0;
float g_touchY = 0;


#define  LOG_TAG  "Asteroids"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

void checkGlError(const char* op) {
  for ( GLint error = glGetError(); error; error
  = glGetError()) {
    LOGI("after %s() glError (0x%x)\n", op, error);
  }
} 

// Called from the app framework.
void appInit()
{
  glEnableClientState(GL_VERTEX_ARRAY);
//  glEnableClientState(GL_COLOR_ARRAY);
}


// Called from the app framework.
void appDeinit()
{
}


void prepareFrame(int width, int height)
{
  glViewport(0, 0, width, height);

  glClear(GL_COLOR_BUFFER_BIT);

  // Make OpenGL coordinates match screen cooridnates
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glScalef(2.0f / width, -2.0f / height, 1.0f);
  glTranslatef(-width / 2.0f, -height / 2.0f, 0);
}


void drawMy()
{
  const float w = 200.0f;

  static GLfloat vertices[4][2];

  float x = g_touchX;
  float y = g_touchY;

  vertices[0][0] = x;
  vertices[0][1] = y - w;
  vertices[1][0] = x;
  vertices[1][1] = y + w;
  vertices[2][0] = x - w;
  vertices[2][1] = y;
  vertices[3][0] = x + w;
  vertices[3][1] = y;

  glVertexPointer(2, GL_FLOAT, 0, vertices);
  checkGlError("glVertexPointer");

  glDrawArrays(GL_LINES, 0, 4);
  checkGlError("glDrawArrays");    
}

// Called from the app framework.
/* The tick is current time in milliseconds, width and height
 * are the image dimensions to be rendered.
 */
void appRender(long tick, int width, int height)
{
  // Prepare OpenGL ES for rendering of the frame.
  prepareFrame(width, height);

  //
  drawMy();
}

void onTouch(float x, float y)
{
  g_touchX = x;
  g_touchY = y;
}
