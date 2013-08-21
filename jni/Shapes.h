#pragma once

#include <vector>

#include "geometry.h"

// Shapes are immutable, they can only be rigidly moved
class CShape
{
public:
  CShape(std::vector<Segment>&& segments);

  Vector GetVelocity() const;
  Point GetCenter() const;
  float GetRadius() const;

  void SetVelocity(Vector velocity);

  void MoveBy(Vector offset);
  void MoveBy(float time);

  virtual void Draw() const;

protected:
  Vector m_velocity;

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

  void ApplyAcceleration(Vector accel);

  void ApplyFriction(float time);

  virtual void Draw() const;

private:
  std::vector<Segment> ConstructSegments() const;

  const float m_mass;
  const float m_friction;

  float m_angle;
};