#include "Joystick.h"

#include <logging/Log.h>
#include <event/JoystickEvent.h>
#include <event/EventDispatcher.h>
#include <cstring>

namespace Greet {
  Joystick::Joystick(uint jsNum, float calibrateLeft, float calibrateRight)
    : m_jsNum(jsNum)
  {
    m_connected = false;
    m_wasConnected = false;
    previousButtons = nullptr;
  }

  Joystick::~Joystick()
  {
    if(previousButtons != nullptr)
      delete[] previousButtons;
  }

  void Joystick::SetState(bool connected)
  {
    if(connected)
    {
      axis = glfwGetJoystickAxes(m_jsNum, &axisCount);
      buttons = glfwGetJoystickButtons(m_jsNum, &buttonCount);
      previousButtons = new unsigned char[buttonCount];
      previousAxis = new float[axisCount];
      memcpy(previousButtons,buttons,buttonCount);
      memcpy(previousAxis,axis,axisCount);
      m_connected = true;
      m_wasConnected = true;
    }
    else
    {
      axis = nullptr;
      buttons = nullptr;
      if(previousButtons != nullptr)
        delete[] previousButtons;
      if(previousAxis != nullptr)
        delete[] previousButtons;
      previousButtons = nullptr;
      previousAxis = nullptr;
      m_connected = false;
      m_wasConnected = false;
    }
  }

  void Joystick::Update()
  {
    m_wasConnected = m_connected;
    if (m_connected)
    {
      int oldAxisCount = axisCount;
      axis = glfwGetJoystickAxes(m_jsNum,&axisCount);
      if(axisCount != oldAxisCount)
      {
        Log::Warning("Joystick axis count changed");
        if(axisCount == 0)
        {
          SetState(false);
          return;
        }
        else
        {
          delete[] previousAxis;
          previousAxis = new float[axisCount];
        }
      }

      for (int i = 0; i < axisCount;i++)
      {
        if(axis[i] != previousAxis[i])
        {
          EventDispatcher::OnEvent(JoystickTriggerMoveEvent{m_jsNum, i, axis[i], axis[i] - previousAxis[i]});
          previousAxis[i] = axis[i];
        }
      }

      int count;
      buttons = glfwGetJoystickButtons(m_jsNum,&count);
      if(buttonCount != count)
      {
        Log::Warning("Button count changed");
        if(count == 0)
        {
          SetState(false);
          return;
        }
        else
        {
          delete[] previousButtons;
          previousButtons = new unsigned char[count];
          buttonCount = count;
        }
      }

      for (int i = 0; i < buttonCount;i++)
      {
        if(previousButtons[i] != buttons[i])
        {
          previousButtons[i] = buttons[i];
          if(buttons[i] == GLFW_PRESS)
            EventDispatcher::OnEvent(JoystickPressEvent{m_jsNum, i});
          else
            EventDispatcher::OnEvent(JoystickReleaseEvent{m_jsNum, i});
        }
      }
    }
  }

  bool Joystick::CheckConnect()
  {
    m_wasConnected = m_connected;
    m_connected = glfwJoystickPresent(m_jsNum);
    return m_connected;
  }
}
