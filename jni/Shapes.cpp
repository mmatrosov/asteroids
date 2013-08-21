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
}

//////////////////////////////////////////////////////////////////////////
///
CShape::CShape(CShape&& that)
{
  m_segments = std::move(that.m_segments);
}

//////////////////////////////////////////////////////////////////////////
///
void CShape::MoveBy(Vector v)
{
  for (Segment& s : m_segments)
  {
    s.a += v;
    s.b += v;
  }

  m_center += v;
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
Point CShape::GetCenter() const
{
  return m_center;
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

  std::vector<Segment> segments;
  Segment s;

  // Left side
  s.a.x = -2 * size;
  s.a.y = -3 * size;
  s.b.x = 0;
  s.b.y = 3 * size;
  segments.push_back(s);

  // Right side
  s.a.x = -s.a.x;
  segments.push_back(s);

  // Aft
  s.a.x = -5.0f / 3 * size;
  s.a.y = -2 * size;
  s.b.x = -s.a.x;
  s.b.y = s.a.y;
  segments.push_back(s);

  return segments;
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