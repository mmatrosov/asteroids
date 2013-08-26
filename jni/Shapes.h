#pragma once

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
  virtual void ApplyTime(float time);

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

  float GetAngle() const;

  void SetAngle(float angle);

  void ApplyAcceleration(Vector accel);

  void ApplyFriction(float time);

  virtual void MoveBy(Vector offset);
  virtual void ApplyTime(float time);

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

class CAsteroid : public CShape
{
public:
  CAsteroid(int degree = 0);

  int GetDegree() const;

private:
  int m_degree;
};

class CProjectile : public CShape
{
public:
  CProjectile();

  virtual void ApplyTime(float time);

  bool IsExpired() const;

private:
  float m_livedTime;
};

CShape CreateStarShape(int vertsCount, float minRadius, float maxRadius = -1);

bool Intersects(const CShape& shape1, const CShape& shape2);