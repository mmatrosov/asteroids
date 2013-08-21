#include "Application.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <random>

#include "gl.h"
#include "Utils.h"

//////////////////////////////////////////////////////////////////////////
///
CApplication::CApplication() : m_maxAsteroids(4)
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
int CApplication::GetScreenWidth() const
{
  return m_width;
}

//////////////////////////////////////////////////////////////////////////
///
int CApplication::GetScreenHeight() const
{
  return m_height;
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::OnResize(int width, int height)
{
  // Assumed to occur only once at application start since the orientation is locked
  m_width = width;
  m_height = height;

  InitMenuShapes();
  InitShip();
  InitAsteroids();
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::OnTouch(float x, float y)
{
  m_touchX = x;
  m_touchY = y;

  m_joystickDir = Point(x, y) - m_pJoystick->GetCenter();
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::Render()
{
  HandleControls();

  PrepareFrame();

  RenderTouch();
  RenderMenu();
  RenderShip();
  RenderAsteroids();
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

  // Place joystick in the bottom left corner
  m_pJoystick->MoveBy(Vector(radius + border, m_height - radius - border));
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::InitShip()
{
  m_pShip.reset(new CShip());

  // Place to the center of the screen
  m_pShip->MoveBy(Vector(m_width / 2.0f, m_height / 2.0f));
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::InitAsteroids()
{
  const int vertsCount = 10;
  const float maxRadius = 100;

  std::vector<Point> verts(vertsCount);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> radiusDist(0, maxRadius);
  std::uniform_real_distribution<float> offsetDist(0, static_cast<float>(m_width));

  for (int i = 0; i < m_maxAsteroids; ++i)
  {
    // Init vertices
    for (int j = 0; j < vertsCount; ++j)
    {
      float radius = radiusDist(gen);
      verts[j].x = static_cast<float>(radius * cos(2 * M_PI * j / vertsCount));
      verts[j].y = static_cast<float>(radius * sin(2 * M_PI * j / vertsCount));
    }

    // Construct segments
    std::vector<Segment> segments;

    auto pV1 = verts.end() - 1;
    auto pV2 = verts.begin();

    for ( ; pV2 != verts.end(); pV1 = pV2++)
    {
      segments.push_back(Segment(*pV1, *pV2));
    }

    CShape asteroid(std::move(segments));

    // Move asteroid randomly, but make sure it doesn't collide with the ship
    bool collision = true;
    while (collision)
    {
      asteroid.MoveBy(Vector(offsetDist(gen), offsetDist(gen)));
      collision = (asteroid.GetCenter() - m_pShip->GetCenter()).len() <= asteroid.GetRadius() + m_pShip->GetRadius();
    };

    // Add an asteroid
    m_asteroids.push_back(std::move(asteroid));
  }

  // Next turn will contain one more asteroid
  m_maxAsteroids++;
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::HandleControls()
{
  if (m_joystickDir.len() < m_pJoystick->GetRadius())
  {
    if (m_joystickDir.len() > 0)
    {
      // Negate angle since axis y is pointing downwards
      m_pShip->SetAngle(-m_joystickDir.angle());
    }

    m_pShip->ApplyAcceleration(m_joystickDir);
  }

  // ToDo: use actual FPS here
  float time = 1.0f / 60;

  m_pShip->MoveBy(time);
  m_pShip->ApplyFriction(time);

  m_joystickDir = Vector();
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

//////////////////////////////////////////////////////////////////////////
///
void CApplication::RenderAsteroids()
{
  for (const CShape& asteroid : m_asteroids)
  {
    asteroid.Draw();
  }
}