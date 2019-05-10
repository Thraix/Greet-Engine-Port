#pragma once

#include <internal/GreetGL.h>
#include <internal/GreetTypes.h>
#include <input/Joystick.h>
#include <math/Maths.h>
#include <memory>

#include <vector>

#define MAX_KEYS			1024
#define MAX_MOUSEBUTTONS	32

#define BUTTON_STATE_PRESSED	0x0
#define BUTTON_STATE_RELEASED	0x1
#define BUTTON_STATE_DOWN		0x2
#define BUTTON_STATE_UP			0x3

#define JOYSTICK_STATE_DISCONNECTED 0x0
#define JOYSTICK_STATE_CONNECTED	0x1

namespace Greet {
  class GUI;
  typedef void(*WINDOW_RESIZE)(int width, int height);
  typedef void(*JOYSTICK_STATE)(uint joystick, unsigned char state);
  typedef void(*WINDOW_FOCUS)(int state); 


  class Window
  {
    private:
      static void* pointer;
      friend struct GLFWwindow;
      static std::string title;
      static uint width, height;
      static GLFWwindow *window;
      static Vec4 bgColor;

      static std::vector<std::unique_ptr<Joystick>> joysticks;

      static bool focus;
      static bool mouseButtonDown[MAX_MOUSEBUTTONS];
      static bool isMouseButtonDown;
      static Vec2 mousePos;
      static Vec2 mousePosPixel;
    private:
      static bool Init();

      static void window_resize(GLFWwindow *window, int width, int height);
      static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
      static void key_char_callback(GLFWwindow* window, uint charCode);

      static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
      static void window_focus_callback(GLFWwindow* window, int state);
      static void window_position_callback(GLFWwindow* window, int x, int y);
      static void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);
      static void mouse_scroll_callback(GLFWwindow* window, double scrollX, double scrollY);
      static void joystick_callback(int joy, int event);
    public:
      //Window(const char *title, int width, int height);
      //~Window();

      static void CreateWindow(std::string title, uint width, uint height);
      static void DestroyWindow();

      static bool Closed();
      static void Clear();

      static void Render();
      static void Update();
      static void Tick();

      static void SetBackgroundColor(Vec4 color);
      static void GrabMouse(bool grab);

      static void TransformMousePosToScreen(Vec2& pos);
      inline static Vec4 GetBackgroundColor() { return bgColor; }
      inline static int GetWidth() { return width; };
      inline static int GetHeight() { return height; };
      inline static float GetAspect() { return width / (float) height; }
      inline static void SetTitle(const std::string& title) { glfwSetWindowTitle(window,title.c_str()); }

      //Mouse Positions
      inline static Vec2 GetMousePos() { return mousePos; }
      inline static float GetMouseX() { return mousePos.x; }
      inline static float GetMouseY() { return mousePos.y; }

      inline static Vec2 GetMousePosPixel() { return mousePosPixel; }
      inline static float GetMouseXPixel() { return mousePosPixel.x; }
      inline static float GetMouseYPixel() { return mousePosPixel.y; }
      inline static Joystick* GetJoystick(uint joystick){ ASSERT(joystick < GLFW_JOYSTICKS, "WINDOW","Invalid Joystick. Ranges from 0-3: ", joystick); return (joysticks[joystick]).get(); }
      inline static bool IsJoystickConnected(uint joystick){ ASSERT(joystick < GLFW_JOYSTICKS, "WINDOW", "Invalid Joystick. Ranges from 0-3: ", joystick); return joysticks[joystick]->m_connected; }


      inline static void* getUserPointer(Window* window) { return window->pointer; }

      static void setUserPointer(Window* window, void* pointer);

  };
}
