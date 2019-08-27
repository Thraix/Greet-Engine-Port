#include "RenderCommand.h"

#include <graphics/Window.h>
#include <logging/Log.h>

namespace Greet
{
  std::stack<Vec4> RenderCommand::viewportStack;

  void RenderCommand::PushViewportStack(float x, float y, float width, float height)
  {
    PushViewportStack({x,y,width,height});
  }

  void RenderCommand::PushViewportStack(const Vec4& viewport)
  {
    const Vec4& lastViewport = viewportStack.empty() 
      ? Vec4(0, 0, Window::GetWidth(), Window::GetHeight()) 
      : viewportStack.top();

    Vec4 vp{viewport};
    vp.x += lastViewport.x;
    vp.y = lastViewport.y + lastViewport.w - viewport.y - viewport.w;
    glViewport(vp.x, vp.y, vp.z, vp.w);
    viewportStack.push(vp);
  }

  void RenderCommand::PopViewportStack()
  {
    if(viewportStack.empty())
    {
      Log::Warning("Trying to pop empty viewport stack");
      return;
    }
    viewportStack.pop();

    // Use window size as default viewport if stack is empty
    const Vec4& vp = viewportStack.empty() 
      ? Vec4(0, 0, Window::GetWidth(), Window::GetHeight()) 
      : viewportStack.top();

    glViewport(vp.x, vp.y, vp.z, vp.w);
  }

  const Vec4& RenderCommand::TopViewportStack()
  {
    return viewportStack.top();
  }
}

