#include "Window.h"

#include <graphics/fonts/FontManager.h>
#include <graphics/textures/TextureManager.h>
#include <graphics/atlas/AtlasManager.h>
#include <internal/GreetGL.h>
#include <event/EventDispatcher.h>
#include <graphics/RenderEngine.h>
#include <graphics/gui/ComponentFactory.h>
#include <event/WindowEvent.h>
#include <event/KeyEvent.h>
#include <event/MouseEvent.h>

namespace Greet {

  std::vector<Joystick> Window::joysticks;
  bool Window::focus;
  Vec2 Window::mousePos;
  Vec2 Window::mousePosPixel;
  uint Window::width;
  uint Window::height;
  std::string Window::title;
  GLFWwindow *Window::window;
  Vec4 Window::bgColor;
  bool Window::mouseButtonDown[MAX_MOUSEBUTTONS];
  bool Window::isMouseButtonDown;
  uint Window::joystickCheck;


  std::vector<WindowResizeListener*> Window::windowResize;
  std::vector<WindowFocusListener*> Window::windowFocus;
  std::vector<JoystickStateListener*> Window::joystickState;

  void Window::CreateWindow(std::string title, uint width, uint height)
  {
    bgColor = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
    focus = true;
    Window::title = title;
    Window::width = width;
    Window::height = height;

    memset(mouseButtonDown,false,MAX_MOUSEBUTTONS);
    for (int i = 0; i < MAX_JOYSTICKS; i++)
    {
      joysticks.push_back(Joystick(i, 0.3f, 0.3f));
    }
    if (!init())
    {
      glfwTerminate();
    }
  }

  void Window::DestroyWindow()
  {
    FontManager::Destroy();
    TextureManager::Destroy();
    AtlasManager::Destroy();
    ComponentFactory::Cleanup();
    FrameFactory::Cleanup();
    glfwTerminate();
  }

  bool Window::init()
  {
    ASSERT(glfwInit(),"Failed to initialize GLFW!");
    glfwWindowHint(GLFW_SAMPLES, 4);
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    ASSERT(window,"Failed to initialize window!");
    //glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);

    //glfwSetWindowUserPointer(window,this);

    glfwSetFramebufferSizeCallback(window, window_resize);
    glfwSetKeyCallback(window,key_callback);
    glfwSetMouseButtonCallback(window,mouse_button_callback);
    glfwSetCursorPosCallback(window,mouse_position_callback);
    glfwSetWindowFocusCallback(window,window_focus_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);
    glfwSetCharCallback(window, key_char_callback);
    glfwSetWindowPosCallback(window, window_position_callback);
    glfwSetJoystickCallback(joystick_callback);

    glfwSwapInterval(0);
    GLCall(glEnable(GL_TEXTURE_2D));
    GLCall(glEnable(GL_BLEND));
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_MULTISAMPLE));
    ASSERT(glewInit() == GLEW_OK,"Glew failed to init.");
    GLCall(glProvokingVertex(GL_FIRST_VERTEX_CONVENTION));

    Log::Info("OpenGL Version: ", glGetString(GL_VERSION));
    Log::Info("GLFW Version: ", glfwGetVersionString());
    return true;
  }

  bool Window::Closed()
  {
    return glfwWindowShouldClose(window) == 1;
  }

  void Window::Clear()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void Window::Tick()
  {

  }

  void Window::Update()
  {
    if (focus){
      for (int i = MAX_JOYSTICKS-1; i >= 0; i--)
      {
        if (joysticks[i].m_connected)
        {
          joysticks[i].Update();
          if (joystickState.size()>0 && !joysticks[i].m_connected && joysticks[i].m_wasConnected)
          {
            for (uint j = 0;j < joystickState.size();j++)
              joystickState[j]->JoystickState(j, JOYSTICK_STATE_DISCONNECTED);
            joystickCheck = i;
          }
        }
      }
    }
  }

  void Window::Render()
  {
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  void Window::SetBackgroundColor(Vec4 color)
  {
    bgColor = color;
    GLCall(glClearColor(color.x, color.y, color.z, color.w));
  }

  void Window::AddResizeCallback(WindowResizeListener* listener)
  {
    windowResize.push_back(listener);
  }

  void Window::RemoveResizeCallback(WindowResizeListener* listener)
  {

    windowResize.erase(std::remove(windowResize.begin(), windowResize.end(), listener));
  }

  void Window::AddWindowFocusCallback(WindowFocusListener* listener)
  {
    windowFocus.push_back(listener);
  }

  void Window::RemoveWindowFocusCallback(WindowFocusListener* listener)
  {
    windowFocus.erase(std::remove(windowFocus.begin(), windowFocus.end(), listener));
  }

  void Window::AddJoystickCallback(JoystickStateListener* listener)
  {
    joystickState.push_back(listener);
  }

  void Window::RemoveJoystickCallback(JoystickStateListener* listener)
  {
    joystickState.erase(std::remove(joystickState.begin(), joystickState.end(), listener));
  }

  void Window::window_resize(GLFWwindow *window, int width, int height)
  {
    glViewport(0, 0, width, height);
    Window::width = width;
    Window::height = height;
    EventDispatcher::OnEvent(WindowResizeEvent{width,height});
    for (uint i = 0;i < Window::windowResize.size();i++)
      windowResize[i]->WindowResize(width,height);
  }

  void Window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
  {
    if (action == GLFW_RELEASE)
      EventDispatcher::OnEvent(KeyReleaseEvent(key));
    else if(action == GLFW_PRESS)
      EventDispatcher::OnEvent(KeyPressEvent(key,0));
    else if(action == GLFW_REPEAT)
      EventDispatcher::OnEvent(KeyPressEvent(key,1));
  }

  void Window::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
  {
    mouseButtonDown[button] = action == GLFW_PRESS;
    if (action == GLFW_RELEASE)
      EventDispatcher::OnEvent(MouseReleaseEvent(mousePosPixel.x,mousePosPixel.y,button));
    else if (action == GLFW_PRESS)
      EventDispatcher::OnEvent(MousePressEvent(mousePosPixel.x, mousePosPixel.y, button));
    isMouseButtonDown = mouseButtonDown[button];
    if(!isMouseButtonDown)
    {
      for (uint i = 0;i < MAX_MOUSEBUTTONS;i++)
      {
        if (mouseButtonDown[i])
        {
          isMouseButtonDown = true;
        }
      }
    }
  }

  void Window::mouse_position_callback(GLFWwindow* window, double xpos, double ypos)
  {
    EventDispatcher::OnEvent(MouseMoveEvent{(float)xpos, (float)ypos, (float)(xpos - mousePosPixel.x), (float)(ypos - mousePosPixel.y)});
    Vec2 mousePosDelta = mousePos;
    mousePos = Vec2(xpos / width, 1.0f - (ypos / height))*2.0f - 1.0f;
    mousePosPixel = Vec2(xpos, ypos);
  }

  void Window::mouse_scroll_callback(GLFWwindow* window, double scrollX, double scrollY)
  {
    EventDispatcher::OnEvent(MouseScrollEvent{(float)scrollX, (float)scrollY});
  }

  void Window::key_char_callback(GLFWwindow* window, uint charCode)
  {
    EventDispatcher::OnEvent(KeyTypeEvent{charCode});
  }

  void Window::window_position_callback(GLFWwindow* window, int x, int y)
  {
    static int xLast = x, yLast = y;
    EventDispatcher::OnEvent(WindowMoveEvent{x,y, xLast, yLast});
    xLast = x;
    yLast = y;
  }

  void Window::window_focus_callback(GLFWwindow* window,int state)
  {
    focus = state == GL_TRUE;
    if(focus)
      EventDispatcher::OnEvent(WindowFocusEvent{});
    else
      EventDispatcher::OnEvent(WindowUnfocusEvent{});
  }

  void Window::joystick_callback(int joy, int event)
  {
    joysticks[joy].SetState(event == GLFW_CONNECTED);
    for (uint i = 0;i < joystickState.size();i++)
      joystickState[i]->JoystickState(joy, event == GLFW_CONNECTED);
  }
}
