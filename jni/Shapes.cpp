#include "Shapes.h"

#include "gl.h"
#include "Utils.h"

#define _USE_MATH_DEFINES
#include "math.h"

float rad2deg(float rad)
{
  return static_cast<float>(rad / M_PI * 180);
}

float deg2rad(float deg)
{
  return static_cast<float>(deg / 180 * M_PI);
}

//////////////////////////////////////////////////////////////////////////
///
CShape::CShape(std::vector<Segment>&& segments)
{
  m_segments = std::move(segments);

  // Init radius
  m_radius = 0;
  for (const Segment& s : m_segments)
  {
    m_radius = std::max(m_radius, static_cast<float>(sqrt(s.a.x * s.a.x + s.a.y * s.a.y)));
    m_radius = std::max(m_radius, static_cast<float>(sqrt(s.b.x * s.b.x + s.b.y * s.b.y)));
  }
}

//////////////////////////////////////////////////////////////////////////
///
Vector CShape::GetVelocity() const
{
  return m_velocity;
}
//////////////////////////////////////////////////////////////////////////
///
Point CShape::GetCenter() const
{
  return m_center;
}

//////////////////////////////////////////////////////////////////////////
///
float CShape::GetRadius() const
{
  return m_radius;
}

//////////////////////////////////////////////////////////////////////////
///
void CShape::SetVelocity(Vector velocity)
{
  m_velocity = velocity;
}

//////////////////////////////////////////////////////////////////////////
///
void CShape::MoveBy(Vector offset)
{
  for (Segment& s : m_segments)
  {
    s.a += offset;
    s.b += offset;
  }

  m_center += offset;
}

//////////////////////////////////////////////////////////////////////////
///
void CShape::MoveBy(float time)
{
  MoveBy(m_velocity * time);
}

//////////////////////////////////////////////////////////////////////////
///
void CShape::Draw() const
{
  glVertexPointer(2, GL_FLOAT, 0, &m_segments[0]);
  glDrawArrays(GL_LINES, 0, m_segments.size() * 2);
}

//////////////////////////////////////////////////////////////////////////
///
CShip::CShip() : CShape(ConstructSegments())
{
  m_angle = static_cast<float>(M_PI);
}

//////////////////////////////////////////////////////////////////////////
///
std::vector<Segment> CShip::ConstructSegments() const
{
  const float size = 15;

  // Shit has the shape of the letter "A" pointing to the right
  std::vector<Segment> segments;
  Segment s;

  // Left side
  s.a.x = -3 * size;
  s.a.y = -2 * size;
  s.b.x = 3 * size;
  s.b.y = 0;
  segments.push_back(s);

  // Right side
  s.a.y = -s.a.y;
  segments.push_back(s);

  // Aft
  s.a.x = -2 * size;
  s.a.y = -5.0f / 3 * size;
  s.b.x = s.a.x;
  s.b.y = -s.a.y;
  segments.push_back(s);

  return segments;
}

//////////////////////////////////////////////////////////////////////////
///
void CShip::SetAngle(float angle)
{
  m_angle = angle;
}

//////////////////////////////////////////////////////////////////////////
///
void CShip::Draw() const
{
  Point center = GetCenter();

  glPushMatrix();

  glTranslatef(center.x, center.y, 0);
  glRotatef(rad2deg(m_angle), 0, 0, -1);
  glTranslatef(-center.x, -center.y, 0);

  CShape::Draw();

  glPopMatrix(); 
}