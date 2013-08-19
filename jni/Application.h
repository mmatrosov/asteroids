
class CApplication
{
public:
  CApplication();
  ~CApplication();

  void OnResize(int width, int height);
  void OnTouch(float x, float y);

  void Render();

private:
  void PrepareFrame();

  int m_width;
  int m_height;

  float m_touchX;
  float m_touchY;
};