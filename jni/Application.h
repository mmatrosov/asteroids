#pragma once

#include <memory>

#include "Shapes.h"

class CApplication
{
public:
  CApplication();
  ~CApplication();

  void OnResize(int width, int height);
  void OnTouch(float x, float y);

  void Render();

private:
  void InitMenuShapes();
  void InitShip();

  void PrepareFrame();
  void RenderTouch();
  void RenderMenu();
  void RenderShip();

  int m_width;
  int m_height;

  float m_touchX;
  float m_touchY;

  std::unique_ptr<CShip> m_pShip;

  std::vector<CShape> m_menuShapes;
};