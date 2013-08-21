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

  bool m_collision;

  double m_collisionTime;
  double m_lastMovementTime;

  std::map<int, Point> m_pointers;

  std::unique_ptr<CShip> m_pShip;
  std::unique_ptr<CShape> m_pJoystick;
  std::unique_ptr<CShape> m_pFireButton;
  std::vector<CShape> m_asteroids;
};