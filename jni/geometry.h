#pragma once

struct Vector
{
  float x, y;

  Vector(float x, float y) : x(x), y(y) {}
};

struct Point
{
  float x, y;

  Point() : x(0), y(0) {}

  Point& operator+=(const Vector& v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }
};

struct Segment
{
  Point a, b;
};