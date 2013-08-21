#include "geometry.h"

#include <cmath>

//////////////////////////////////////////////////////////////////////////
///
float cross(const Vector& a, const Vector& b)
{
  return a.x * b.y - a.y * b.x;
}

//////////////////////////////////////////////////////////////////////////
///
bool Intersects(const Segment& s1, const Segment& s2)
{
  Vector s = s1.b - s1.a;
  Vector r = s2.b - s2.a;

  float k = cross(r, s);

  if (std::abs(k) < 1e-5)
  {
    return false;
  }

  Vector q = s1.a.ToVector();
  Vector p = s2.a.ToVector();

  Vector d = q - p;

  float t = cross(d, s) / k;
  float u = cross(d, r) / k;

  return t >= 0 && t <= 1 && u >= 0 && u <= 1;
}