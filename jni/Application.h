#pragma once

#include <memory>

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
  void InitMenuShapes();
  void InitShip();

  void HandleControls();

  void PrepareFrame();
  void RenderTouch();
  void RenderMenu();
  void RenderShip();

  int m_width;
  int m_height;

  float m_touchX;
  float m_touchY;

  Vector m_joystickDir;

  std::unique_ptr<CShip> m_pShip;
  std::unique_ptr<CShape> m_pJoystick;
};