#include "GUIUtils.h"

#include <graphics/gui/Component.h>
#include <graphics/gui/Frame.h>
#include <graphics/gui/Button.h>
#include <graphics/gui/Label.h>
#include <graphics/gui/ProgressBar.h>
#include <graphics/gui/Container.h>
#include <graphics/gui/Divider.h>
#include <graphics/gui/TextBox.h>

namespace Greet
{
  Frame* GUIUtils::GetFrame(const XMLObject& object)
  {
    if (object.GetName() == "Frame")
    {
      return new Frame(object);
    }
    else if (object.GetName() == "FrameHeader")
    {

    }
    Log::Warning("Could not read XML object ", object.GetName(), ".");
    return new Frame();
  }

  Component* GUIUtils::GetComponent(const XMLObject& object, Component* parent)
  {
    if (object.GetName() == "Button")
    {
      return new Button(object, parent);
    }
    else if(object.GetName() == "Label")
    {
      return new Label(object, parent);
    }
    else if(object.GetName() == "ProgressBar")
    {
      return new ProgressBar(object,parent);
    }
    else if (object.GetName() == "Component")
    {
      return new Component(object, parent);
    }
    else if (object.GetName() == "Divider")
    {
      return new Divider(object, parent);
    }
    else if (object.GetName() == "TextBox")
    {
      return new TextBox(object, parent);
    }
    else if(object.GetName() == "Container")
    {
      return new Container(object, parent);
    }
    Log::Warning("Could not read XML object ", object.GetName(), ".");
    return new Component(); // Return plain content to avoid crash.
  }
}
