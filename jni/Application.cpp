#include "Application.h"

#include <GLES/gl.h>

//////////////////////////////////////////////////////////////////////////
///
CApplication::CApplication()
{
  glEnableClientState(GL_VERTEX_ARRAY);
}

//////////////////////////////////////////////////////////////////////////
///
CApplication::~CApplication()
{
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::OnResize(int width, int height)
{
  m_width = width;
  m_height = height;
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::OnTouch(float x, float y)
{
  m_touchX = x;
  m_touchY = y;
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::Render()
{
  PrepareFrame();

  const float w = 200.0f;

  static GLfloat vertices[4][2];

  float x = m_touchX;
  float y = m_touchY;

  vertices[0][0] = x;
  vertices[0][1] = y - w;
  vertices[1][0] = x;
  vertices[1][1] = y + w;
  vertices[2][0] = x - w;
  vertices[2][1] = y;
  vertices[3][0] = x + w;
  vertices[3][1] = y;

  glVertexPointer(2, GL_FLOAT, 0, vertices);

  glDrawArrays(GL_LINES, 0, 4);
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::PrepareFrame()
{
  glViewport(0, 0, m_width, m_height);

  glClear(GL_COLOR_BUFFER_BIT);

  // Make OpenGL coordinates match screen coordinates
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glTranslatef(-1, 1, 0);
  glScalef(2.0f / m_width, -2.0f / m_height, 1.0f);
}