#pragma once

#include "Shapes.h"

class CApplication
{
public:
  CApplication();
  ~CApplication();

  int GetScreenWidth() const;
  int GetScreenHeight() const;

  void OnResize(int width, int height);
  void OnTouchDown(int id, float x, float y);
  void OnTouchMove(int id, float x, float y);
  void OnTouchUp(int id);
  void OnTouchCancel();

  void Render();

private:
  void Initialize();
  void InitMenuShapes();
  void InitShip();
  void InitAsteroids();
  void InitProjectiles();

  void HandleControls();
  void HandleProjectile();
  void MoveObjects();

  void PrepareFrame();
  void RenderObjects();

  void HandleCollisions();

  CShape CreateAsteroid(int degree) const;

  bool m_isInitialized;

  int m_maxAsteroids;

  int m_width;
  int m_height;

  bool m_isShipCrashed;

  bool m_wasProjectileFired;

  double m_shipCrashTime;
  double m_lastMovementTime;

  std::map<int, Point> m_pointers;

  std::unique_ptr<CShape> m_pJoystick;
  std::unique_ptr<CShape> m_pFireButton;
  std::unique_ptr<CShip> m_pShip;
  std::list<CAsteroid> m_asteroids;
  std::list<CProjectile> m_projectiles;
};