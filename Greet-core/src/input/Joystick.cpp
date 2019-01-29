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
    previousButtons = NULL;
  }

  Joystick::~Joystick()
  {
    if(previousButtons != NULL)
      delete[] previousButtons;
  }

  void Joystick::SetState(bool connected)
  {
    if(connected)
    {
      axes = glfwGetJoystickAxes(m_jsNum, &axisCount);
      buttons = glfwGetJoystickButtons(m_jsNum, &buttonCount);
      previousButtons = new unsigned char[buttonCount];
      memcpy(previousButtons,buttons,buttonCount);
      m_connected = true;
      m_wasConnected = true;
      Log::Info("Connected Joystick (axes=",axisCount,", buttons=",buttonCount);
    }
    else
    {
      axes = NULL;
      buttons = NULL;
      if(previousButtons != NULL)
        delete[] previousButtons;
      previousButtons = NULL;
      m_connected = false;
      m_wasConnected = false;
    }
  }

  void Joystick::Update()
  {
    m_wasConnected = m_connected;
    if (m_connected)
    {
      axes = glfwGetJoystickAxes(m_jsNum,&axisCount);
      if(axisCount == 0)
      {
        SetState(false);
        return;
      }

      for (int i = 0; i < axisCount;i++)
      {
        EventDispatcher::OnEvent(JoystickTriggerMoveEvent{m_jsNum, i, axes[i], 0});
      }

      int count;
      buttons = glfwGetJoystickButtons(m_jsNum,&count);
      if(buttonCount != count)
      {
        Log::Warning("wrong buttonCount");
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
