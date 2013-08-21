#include "stdafx.h"

#include "Shapes.h"

#include "Globals.h"

float rad2deg(float rad)
{
  return static_cast<float>(rad / PI * 180);
}

float deg2rad(float deg)
{
  return static_cast<float>(deg / 180 * PI);
}

//////////////////////////////////////////////////////////////////////////
///
bool Intersects(const CShape& shape1, const CShape& shape2)
{
  if ((shape1.GetCenter() - shape2.GetCenter()).len() >= shape1.GetRadius() + shape2.GetRadius())
  {
    return false;
  }

  for (const Segment& seg1 : shape1.GetSegments())
  {
    for (const Segment& seg2 : shape2.GetSegments())
    {
      if (Intersects(seg1, seg2))
      {
        return true;
      }
    }
  }

  return false;
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

  // Handle wrapping beside screen boundaries
  Vector wrap;
  int width = Globals::ScreenWidth();
  int height = Globals::ScreenHeight();

  if (m_center.x < -m_radius)
  {
    wrap.x = width + 2 * m_radius;
  } 
  else if (m_center.x > width + m_radius)
  {
    wrap.x = -width - 2 * m_radius;
  }

  if (m_center.y < -m_radius)
  {
    wrap.y = height + 2 * m_radius;
  }
  else if (m_center.y > height + m_radius)
  {
    wrap.y = -height - 2 * m_radius;
  }

  if (wrap.x != 0 || wrap.y != 0)
  {
    // Since we're using strict inequalities, this call won't infinitely recurse
    MoveBy(wrap);
  }
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
const std::vector<Segment>& CShape::GetSegments() const
{
  return m_segments;
}

//////////////////////////////////////////////////////////////////////////
///
CShip::CShip() : 
  CShape(ConstructSegments()), 
  m_mass(10), m_friction(10)
{
  m_angle = PI / 2;
  m_areRotatedSegmentsValid = false;
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
  m_areRotatedSegmentsValid = false;
}

//////////////////////////////////////////////////////////////////////////
///
void CShip::ApplyAcceleration(Vector accel)
{
  m_velocity += accel / m_mass;
}

//////////////////////////////////////////////////////////////////////////
///
void CShip::ApplyFriction(float time)
{
  ApplyAcceleration(GetVelocity() * (-1 * m_friction * time));
}

//////////////////////////////////////////////////////////////////////////
///
void CShip::MoveBy(Vector offset)
{
  CShape::MoveBy(offset);
  m_areRotatedSegmentsValid = false;
}

//////////////////////////////////////////////////////////////////////////
///
void CShip::MoveBy(float time)
{
  CShape::MoveBy(time);
  m_areRotatedSegmentsValid = false;
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

//////////////////////////////////////////////////////////////////////////
///
const std::vector<Segment>& CShip::GetSegments() const
{
  if (!m_areRotatedSegmentsValid)
  {
    // Get raw unrotated segments
    m_rotatedSegments = CShape::GetSegments();

    const float c = cos(m_angle);
    const float s = -sin(m_angle);  // Negated since y axis points downwards

    Vector r = GetCenter().ToVector();

    auto rotate = [&] (Point& p)
    {
      p -= r;
      p = Point(p.x * c - p.y * s, p.x * s + p.y * c);
      p += r;
    };

    for (Segment& s : m_rotatedSegments)
    {
      rotate(s.a);
      rotate(s.b);
    }

    m_areRotatedSegmentsValid = true;
  }

  return m_rotatedSegments;
}