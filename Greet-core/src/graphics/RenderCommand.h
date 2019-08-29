#pragma once

#include <stack>
#include <math/Vec2.h>
#include <math/Vec4.h>

namespace Greet
{
  class RenderCommand 
  {
    private:
      RenderCommand() = delete;
      static std::stack<Vec4> viewportStack;
      static Vec4 clearColor;
    public:
      /*
         Values relative to the current viewport
         position is from the top-left of the screen
         */
      static void PushViewportStack(float x, float y, float width, float height);
      static void PushViewportStack(const Vec2& pos, const Vec2& size);
      static void PushViewportStack(const Vec4& viewport);

      static void PopViewportStack();
      static Vec4 TopViewportStack();

      static float GetViewportWidth();
      static float GetViewportHeight();
      static float GetViewportAspect();

      static void SetClearColor(const Vec4& clearColor);
      static const Vec4& GetClearColor();
      static void Clear();
  };
} 

