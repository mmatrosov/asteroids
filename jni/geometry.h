#pragma once

struct Vector
{
  float x, y;

  Vector() : x(0), y(0) {}
  Vector(float x, float y) : x(x), y(y) {}

  static Vector FromPolar(float rho, float phi)
  {
    return Vector(rho * cos(phi), rho * sin(phi));
  }

  float len() const
  {
    return static_cast<float>(sqrt(x * x + y * y));
  }

  float angle() const
  {
    return static_cast<float>(atan2(y, x));
  }

  Vector norm() const
  {
    float l = len();
    return Vector(x / l, y / l);
  }

  Vector rotate(float angle) const
  {
    const float c = cos(angle);
    const float s = sin(angle);
    return Vector(x * c - y * s, x * s + y * c);
  }

  Vector operator*(float a) const
  {
    return Vector(x * a, y * a);
  }
  Vector operator/(float a) const
  {
    return Vector(x / a, y / a);
  }

  Vector operator+(Vector rhs) const
  {
    return Vector(x + rhs.x, y + rhs.y);
  }
  Vector operator-(Vector rhs) const
  {
    return Vector(x - rhs.x, y - rhs.y);
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
  Point(const Vector& that) : x(that.x), y(that.y) {}

  Vector ToVector() const
  {
    return Vector(x, y);
  }

  Point& operator+=(const Vector& v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }
  Point& operator-=(const Vector& v)
  {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  Vector operator-(const Point& rhs) const
  {
    return Vector(x - rhs.x, y - rhs.y);
  }
};

struct Segment
{
  Point a, b;

  Segment() {}
  Segment(const Point& a, const Point& b) : a(a), b(b) {}

  Segment operator-(const Vector& v)
  {
    return Segment(a - v, b - v);
  }
};

bool Intersects(const Segment& s1, const Segment& s2);

float cross(const Vector& a, const Vector& b);

float rad2deg(float rad);
float deg2rad(float deg);
