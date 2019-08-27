#pragma once

#include <math/Vec2.h>
#include <vector>

namespace Greet
{
  class Input 
  {
    public:
      friend class Window;
    private:
      Input() = delete;

      static Vec2 mousePos;
      static std::vector<bool> mouseButtonsDown;
      static std::vector<bool> keyButtonsDown;
      static float mouseScrollX;
      static float mouseScrollY;

      static void SetMousePos(const Vec2& pos);
      static void SetKeyButton(int keyCode, bool value);
      static void SetMouseButton(int keyCode, bool value);
      static void AddMouseScroll(float scrollX, float scrollY);

    public:

      static bool IsKeyDown(int keyCode);
      static bool IsMouseDown(int keyCode);
      static Vec2 GetMousePos(bool ignoreViewport = false);
      static Vec2 GetMousePosPixel(bool ignoreViewport = false);
      static float GetMouseScrollX();
      static float GetMouseScrollY();

      static void Update();
  };
} 

