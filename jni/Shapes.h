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

  virtual void MoveBy(Vector offset);
  virtual void MoveBy(float time);

  virtual void Draw() const;

  virtual const std::vector<Segment>& GetSegments() const;

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

  virtual void MoveBy(Vector offset);
  virtual void MoveBy(float time);

  virtual void Draw() const;

  virtual const std::vector<Segment>& GetSegments() const;

private:
  std::vector<Segment> ConstructSegments() const;

  const float m_mass;
  const float m_friction;

  float m_angle;

  mutable bool m_areRotatedSegmentsValid;
  mutable std::vector<Segment> m_rotatedSegments;
};

bool Intersects(const CShape& shape1, const CShape& shape2);