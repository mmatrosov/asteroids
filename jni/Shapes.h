#pragma once

#include <vector>

#include "geometry.h"

// Shapes are immutable, they can only be rigidly moved
class CShape
{
public:
  CShape(std::vector<Segment>&& segments);

  void MoveBy(Vector v);

  virtual void Draw() const;

  Point GetCenter() const;
  float GetRadius() const;

private:
  std::vector<Segment> m_segments;
  Point m_center;
  float m_radius;
};

// Ship is a shape that can also rotate
class CShip : public CShape
{
public:
  CShip();

  void SetAngle(float angle);

  virtual void Draw() const;

private:
  std::vector<Segment> ConstructSegments() const;

  float m_angle;
};