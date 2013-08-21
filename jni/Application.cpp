#include "Application.h"

#include "gl.h"
#include "Utils.h"

#define _USE_MATH_DEFINES
#include "math.h"

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

  InitMenuShapes();
  InitShip();
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::OnTouch(float x, float y)
{
  m_touchX = x;
  m_touchY = y;

  Point p(x, y);
  Vector r = p - m_pJoystick->GetCenter();

  if (r.len() < m_pJoystick->GetRadius())
  {
    float angle = -r.angle();
    LOGI("angle=%g", angle);
    // Negate angle since axis y is pointing downwards
    m_pShip->SetAngle(angle);
  }
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::Render()
{
  PrepareFrame();

  RenderTouch();
  RenderMenu();
  RenderShip();
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::InitMenuShapes()
{
  const int count = 8;
  const float radius = 150;
  const float border = 20;

  std::vector<Segment> segments;

  Segment s;

  for (int i = 0; i < count; ++i)
  {
    s.a.x = static_cast<float>(radius * cos(2 * M_PI * i / count));
    s.a.y = static_cast<float>(radius * sin(2 * M_PI * i / count));
    s.b.x = static_cast<float>(radius * cos(2 * M_PI * (i + 1) / count));
    s.b.y = static_cast<float>(radius * sin(2 * M_PI * (i + 1) / count));
    segments.push_back(s);
  }

  m_pJoystick.reset(new CShape(std::move(segments)));

  m_pJoystick->MoveBy(Vector(radius + border, m_height - radius - border));
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::InitShip()
{
  m_pShip.reset(new CShip());

  m_pShip->MoveBy(Vector(m_width / 2.0f, m_height / 2.0f));
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

  // Set model matrix as current
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity(); 
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::RenderTouch()
{
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

  m_touchX = -1;
  m_touchY = -1;
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::RenderMenu()
{
  m_pJoystick->Draw();
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::RenderShip()
{
  m_pShip->Draw();
}