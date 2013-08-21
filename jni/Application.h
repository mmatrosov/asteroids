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
  void OnTouch(float x, float y);

  void Render();

private:
  void Initialize();
  void InitMenuShapes();
  void InitShip();
  void InitAsteroids();

  void HandleControls();
  void MoveObjects();

  void PrepareFrame();
  void RenderTouch();
  void RenderMenu();
  void RenderShip();
  void RenderAsteroids();

  void HandleCollisions();

  bool m_isInitialized;

  int m_maxAsteroids;

  int m_width;
  int m_height;

  float m_touchX;
  float m_touchY;

  Vector m_joystickDir;

  bool m_collision;

  double m_collisionTime;

  std::unique_ptr<CShip> m_pShip;
  std::unique_ptr<CShape> m_pJoystick;
  std::vector<CShape> m_asteroids;
};