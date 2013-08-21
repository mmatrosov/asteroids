#include "stdafx.h"

#include "Application.h"

// Get time in seconds
double GetTime()
{
#ifndef _MSC_VER
  timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  return now.tv_sec + now.tv_nsec / 1e9;
#else
  return 0;
#endif
}

//////////////////////////////////////////////////////////////////////////
///
CApplication::CApplication() : m_maxAsteroids(4)
{
  LOGI("In CApplication()");

  glEnableClientState(GL_VERTEX_ARRAY);

  m_isInitialized = false;

  m_collision = false;
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
  m_width = width;
  m_height = height;

  // Initialization is done once screen dimensions are known
  if (!m_isInitialized)
  {
    Initialize();
  }
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::OnTouchDown(int id, float x, float y)
{
  m_pointers[id] = Point(x, y);
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::OnTouchMove(int id, float x, float y)
{
  m_pointers[id] = Point(x, y);
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::OnTouchUp(int id)
{
  auto p = m_pointers.find(id);

  if (p != m_pointers.end())
  {
    m_pointers.erase(p);
  }
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::OnTouchCancel()
{
  m_pointers.clear();
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::Render()
{
  if (m_collision)
  {
    if (GetTime() - m_collisionTime > 1)
    {
      Initialize();
      m_collision = false;
    }
  }

  if (!m_collision)
  {
    HandleControls();
    MoveObjects();
//    HandleCollisions();
  }

  PrepareFrame();

  RenderTouch();
  RenderMenu();
  RenderShip();
  RenderAsteroids();
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::Initialize()
{
  LOGI("In Initialize()");

  InitMenuShapes();
  InitShip();
  InitAsteroids();

  m_isInitialized = true;
  m_lastMovementTime = GetTime();
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::InitMenuShapes()
{
  const int count = 8;
  const float radius = 150;
  const float border = 20;

  m_pJoystick.reset(new CShape(CreateStarShape(count, radius)));

  // Fire button looks exactly like joystick
  m_pFireButton.reset(new CShape(*m_pJoystick));

  // Place joystick in the bottom left corner
  m_pJoystick->MoveBy(Vector(radius + border, m_height - radius - border));

  // Place fire button in the bottom right corner
  m_pFireButton->MoveBy(Vector(m_width - radius - border, m_height - radius - border));
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
  const float speed = 100;
  const float safeSpace = 200;

  // Asteroids are generated as star polygons
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> offsetDist(0, static_cast<float>(m_width));
  std::uniform_real_distribution<float> angleDist(0, static_cast<float>(2 * PI));

  m_asteroids.clear();

  for (int i = 0; i < m_maxAsteroids; ++i)
  {
    CShape asteroid = CreateStarShape(vertsCount, 0, maxRadius);

    // Move asteroid randomly, but make sure it doesn't collide with the ship
    bool collision = true;

    while (collision)
    {
      asteroid.MoveBy(Vector(offsetDist(gen), offsetDist(gen)));

      float centerLen = (asteroid.GetCenter() - m_pShip->GetCenter()).len();
      float minLen = asteroid.GetRadius() + m_pShip->GetRadius() + safeSpace;

      collision = centerLen <= minLen;
    };

    // Set randomly directed velocity
    asteroid.SetVelocity(Vector::FromPolar(speed, angleDist(gen)));

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
  for (const auto& ptr : m_pointers)
  {
    Vector dir = ptr.second - m_pJoystick->GetCenter();

    if (dir.len() < m_pJoystick->GetRadius())
    {
      // Negate angle since axis y is pointing downwards
      m_pShip->SetAngle(-dir.angle());

      m_pShip->ApplyAcceleration(dir);

      // Only first pointer is taken into account. It is unlikely that joystick will be
      // manipulated with more than one finger.
      break;
    }
  }
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::MoveObjects()
{
  float time = static_cast<float>(GetTime() - m_lastMovementTime);

  m_pShip->MoveBy(time);
  m_pShip->ApplyFriction(time);

  for (CShape& asteroid : m_asteroids)
  {
    asteroid.MoveBy(time);
  }

  m_lastMovementTime = GetTime();
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
  // Seems like this function is causing an error eventually:
  // A/libc(5821): Fatal signal 11 (SIGSEGV) at 0x00070013 (code=1)
  return;

  const float w = 200.0f;

  static GLfloat vertices[4][2] = { 0 };

  vertices[0][1] = -w;
  vertices[1][1] = w;
  vertices[2][0] = -w;
  vertices[3][0] = w;

  glVertexPointer(2, GL_FLOAT, 0, vertices);

  for (const auto& ptr : m_pointers)
  {
    Point p = ptr.second;

    glPushMatrix();
    glTranslatef(p.x, p.y, 0);
    glDrawArrays(GL_LINES, 0, 4);
    glPopMatrix();
  }
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::RenderMenu()
{
  m_pJoystick->Draw();
  m_pFireButton->Draw();
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

//////////////////////////////////////////////////////////////////////////
///
void CApplication::HandleCollisions()
{
  for (const CShape& asteroid : m_asteroids)
  {
    if (Intersects(asteroid, *m_pShip))
    {
      m_collision = true;
      m_collisionTime = GetTime();
      break;
    }
  }
}