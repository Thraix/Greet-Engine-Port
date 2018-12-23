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

  void RadioButton::Render(GUIRenderer* renderer) const
  {
    return;
    if(active)
      renderer->SubmitRect(pos, Vec2(20,20), Vec4(1,1,1,1), false);
    else
      renderer->SubmitRect(pos, Vec2(20,20), Vec4(1,0,0,1), false);
  }

  void RadioButton::CallOnClickCallback() 
  {
    RadioGroup* group = dynamic_cast<RadioGroup*>(parent);
    if(group)
      group->NotifyRadioChange(this);
    else
      Log::Warning("RadioButton is not inside a RadioGroup");

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
