#pragma once

#include <math.h>

struct Vector
{
  float x, y;

  Vector() : x(0), y(0) {}
  Vector(float x, float y) : x(x), y(y) {}

  float len() const
  {
    return static_cast<float>(sqrt(x * x + y * y));
  }

  float angle() const
  {
    return static_cast<float>(atan2(y, x));
  }

  Vector operator*(float a)
  {
    Vector v(x * a, y * a);
    return v;
  }
  Vector operator/(float a)
  {
    Vector v(x / a, y / a);
    return v;
  }

  Vector& operator+=(const Vector& rhs)
  {
    x += rhs.x;
    y += rhs.y;
    return *this;
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