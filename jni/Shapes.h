#pragma once

#include <vector>

#include "geometry.h"

class CShape
{
public:
  CShape(std::vector<Segment>&& segments);

  CShape(CShape&& that);

  void MoveBy(Vector v);

  virtual void Draw() const;

protected:
  Point GetCenter() const;

private:
  std::vector<Segment> m_segments;
  Point m_center;
};

class CShip : public CShape
{
public:
  CShip();

  void SetRotation(float angle);

  virtual void Draw() const;

private:
  std::vector<Segment> ConstructSegments() const;

  float m_angle;
};