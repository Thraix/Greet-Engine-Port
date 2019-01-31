#pragma once

#include <graphics/shaders/Shader.h>
#include <listeners/WindowResize.h>
#include <graphics/layers/Scene.h>
#include <graphics/renderers/GUIRenderer.h>
#include <event/Event.h>
#include <event/KeyEvent.h>
#include <event/MouseEvent.h>
#include <string>
#include <map>

namespace Greet {

  class Frame;
  class Component;

  class GUIScene : public Scene
  {
    private:
      Mat3 projectionMatrix;
      std::vector<Frame*> frames;
      Component* m_focused;
      GUIRenderer* m_renderer;
      Shader m_shader;

    public:
      GUIScene(GUIRenderer* renderer, Shader&& shader);

      void OnEvent(Event& event);
      void OnPressed(MousePressEvent& event);
      void OnReleased(MouseReleaseEvent& event);
      void OnMoved(MouseMoveEvent& event);
      void OnPressed(KeyPressEvent& event);
      void OnReleased(KeyReleaseEvent& event);
      void OnTyped(KeyTypeEvent& event);
      void WindowResize(WindowResizeEvent& event);

      void Render() const;
      void Update(float timeElapsed);

      bool RequestFocus(Component* component);
      void AddFrame(Frame* frame);
      Frame* RemoveFrame(const std::string& name);
      Frame* RemoveFrame(Frame* frame);
      Frame* GetFrame(const std::string& name);

      float GetWidth();
      float GetHeight();

  };
}
