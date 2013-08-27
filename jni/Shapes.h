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

  float m_angle;

  mutable bool m_areRotatedSegmentsValid;
  mutable std::vector<Segment> m_rotatedSegments;
};

// Asteroid is a shape with tracked generation number - a degree
class CAsteroid : public CShape
{
public:
  CAsteroid(int degree = 0);

  int GetDegree() const;

  std::vector<CAsteroid> CreateShatters() const;

private:
  static CShape CreateAsteroid(int degree);

  static const int SHATTERS_COUNT = 2;
  static const int MAX_DEGREE = 2;

  int m_degree;
};

// Projectile expires after a certain amount of time. It also leaves shadow track for
// better collision detection, since it is very small.
class CProjectile : public CShape
{
public:
  CProjectile();

  bool IsExpired() const;

  virtual void MoveBy(Vector offset);
  virtual void ApplyTime(float time);

  virtual const std::vector<Segment>& GetSegments() const;

private:
  float m_livedTime;
  float m_lastMoveTime;

  mutable bool m_areShadowSegmentsValid;
  mutable std::vector<Segment> m_shadowSegments;
};

CShape CreateStarShape(int vertsCount, float minRadius, float maxRadius = -1);

bool Intersects(const CShape& shape1, const CShape& shape2);