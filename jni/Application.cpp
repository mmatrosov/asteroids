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
  m_isShipCrashed = false;
  m_wasProjectileFired = false;
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

  // A projectile should only be fired on new touch
  if ((Point(x, y) - m_pFireButton->GetCenter()).len() < m_pFireButton->GetRadius())
  {
    m_wasProjectileFired = true;
  }
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
  if (m_isShipCrashed)
  {
    if (GetTime() - m_shipCrashTime > 1)
    {
      Initialize();
      m_isShipCrashed = false;
    }
  }

  // All asteroids were destroyed
  if (m_asteroids.empty())
  {
    // Next turn will contain one more asteroid
    m_maxAsteroids++;
    Initialize();
  }

  if (!m_isShipCrashed)
  {
    HandleControls();
    HandleProjectile();
    MoveObjects();
    HandleCollisions();
  }

  PrepareFrame();
  RenderObjects();
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
  const float radius = 150;
  const float border = 20;

  // Place joystick in the bottom left corner
  m_pJoystick.reset(new CShape(CreateStarShape(8, radius)));
  m_pJoystick->MoveBy(Vector(radius + border, m_height - radius - border));

  // Place fire button in the bottom right corner
  m_pFireButton.reset(new CShape(CreateStarShape(8, radius)));
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
    CAsteroid asteroid(0);

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
void CApplication::HandleProjectile()
{
  bool exitFlag = !m_wasProjectileFired;

  m_wasProjectileFired = false;

  if (exitFlag)
  {
    return;
  }

  const float speed = 500;

  CProjectile projectile;

  Vector shipDir = Vector::FromPolar(m_pShip->GetRadius(), -m_pShip->GetAngle());
  Vector shipVel = m_pShip->GetVelocity();

  // A projectile is shot from the front of the ship with it's own speed
  projectile.MoveBy(m_pShip->GetCenter().ToVector() + shipDir);
  projectile.SetVelocity(shipVel + shipDir.norm() * speed);

  m_projectiles.push_back(std::move(projectile));
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::MoveObjects()
{
  float time = static_cast<float>(GetTime() - m_lastMovementTime);

  m_pShip->ApplyTime(time);
  m_pShip->ApplyFriction(time);

  for (CShape& asteroid : m_asteroids)
  {
    asteroid.ApplyTime(time);
  }

  m_projectiles.erase(
    std::remove_if(m_projectiles.begin(), m_projectiles.end(), 
      [] (const CProjectile& p) { return p.IsExpired(); }), 
    m_projectiles.end());

  for (CShape& projectile : m_projectiles)
  {
    projectile.ApplyTime(time);
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
void CApplication::RenderObjects()
{
  m_pJoystick->Draw();
  m_pFireButton->Draw();
  m_pShip->Draw();

  for (const CShape& asteroid : m_asteroids)
  {
    asteroid.Draw();
  }

  for (const CShape& projectile : m_projectiles)
  {
    projectile.Draw();
  }
}

//////////////////////////////////////////////////////////////////////////
///
void CApplication::HandleCollisions()
{
  // Ship with asteroids collisions
  for (const CShape& asteroid : m_asteroids)
  {
    if (Intersects(asteroid, *m_pShip))
    {
//      m_isShipCrashed = true;
      m_shipCrashTime = GetTime();
      break;
    }
  }

  // Projectiles with asteroids collisions
  for (auto pAsteroid = m_asteroids.begin(); pAsteroid != m_asteroids.end(); )
  {
    bool isHit = false;

    for (auto pProjectile = m_projectiles.begin(); pProjectile != m_projectiles.end(); ++pProjectile)
    {
      if (Intersects(*pAsteroid, *pProjectile))
      {
        isHit = true;
        m_projectiles.erase(pProjectile);
        break;
      }
    }

    if (!isHit)
    {
      ++pAsteroid;
    }
    else
    {
      auto shatters = pAsteroid->CreateShatters();

      std::move(shatters.begin(), shatters.end(), std::back_inserter(m_asteroids));

      pAsteroid = m_asteroids.erase(pAsteroid);
    }
  }
}