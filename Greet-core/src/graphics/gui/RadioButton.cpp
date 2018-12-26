#include "RadioButton.h"

#include <graphics/gui/RadioGroup.h>

namespace Greet
{
  RadioButton::RadioButton(const XMLObject& xmlObject, Component* parent)
    : Component(xmlObject, parent), active(false)
  {
    m_isFocusable = true;
    activeStyle.Load("active", *this, &normalStyle);
  }

  void RadioButton::PostConstruction()
  {
    radioParent = dynamic_cast<RadioGroup*>(parent);
    if(!radioParent)
      Log::Warning("RadioButton is not inside a RadioGroup");
  }

  void RadioButton::CallOnClickCallback() 
  {
    if(radioParent)
      radioParent->NotifyRadioChange(this);
    Select(true);
    Component::CallOnClickCallback();
  }

  void RadioButton::Select(bool select)
  {
    active = select;
    if(active)
      currentStyle = &activeStyle;
    else
      currentStyle = &normalStyle;
  }
}
