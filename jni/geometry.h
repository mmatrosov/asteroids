#pragma once

#include <math.h>

struct Vector
{
  float x, y;

  Vector(float x, float y) : x(x), y(y) {}

  float len() const
  {
    return static_cast<float>(sqrt(x * x + y * y));
  }

  float angle() const
  {
    return static_cast<float>(atan2(y, x));
  }
};

struct Point
{
  float x, y;

  Point() : x(0), y(0) {}
  Point(float x, float y) : x(x), y(y) {}

  Point& operator+=(const Vector& v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }

  Vector operator-(const Point& rhs)
  {
    Vector v(x - rhs.x, y - rhs.y);
    return v;
  }
};

struct Segment
{
  Point a, b;
};