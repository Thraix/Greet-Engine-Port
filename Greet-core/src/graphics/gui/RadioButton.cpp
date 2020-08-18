#include "RadioButton.h"

#include <graphics/gui/RadioGroup.h>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(RadioButton);

  RadioButton::RadioButton(const XMLObject& xmlObject, Component* parent)
    : Component(xmlObject, parent), active(false), radioParent{nullptr}, text{xmlObject.GetText()}
  {
    AddStyle("active", "normal");
    LoadStyles(xmlObject);
  }

  void RadioButton::PostConstruction()
  {
    Component::PostConstruction();
    radioParent = FindRadioGroupParent();
    if(!radioParent)
      Log::Warning("RadioButton is not inside a RadioGroup");
  }

  void RadioButton::CallOnClickCallback()
  {
    if(radioParent)
      radioParent->NotifyRadioChange(this);
    else
      Log::Error("No RadioGroup parent");
    Select(true);
    Component::CallOnClickCallback();
  }

  void RadioButton::Select(bool select)
  {
    active = select;
    if(active)
      SetCurrentStyle("active");
    else
      SetCurrentStyle("normal");
  }

  float RadioButton::GetWrapWidth() const
  {
    return text.GetWrapWidth();
  }

  float RadioButton::GetWrapHeight() const
  {
    float width = text.font.GetWidthOfText(text.str);
    float maxWidth = GetWidthType() != GUISize::Type::Wrap ? Math::Min(width, GetContentSize().w) : width;
    return text.GetWrapHeight(maxWidth);
  }

  RadioGroup* RadioButton::FindRadioGroupParent() const
  {
    Component* p = parent;
    RadioGroup* rp = dynamic_cast<RadioGroup*>(p);
    while(!rp && p->GetParent())
    {
      p = p->GetParent();
      rp = dynamic_cast<RadioGroup*>(p);
    }
    return rp;
  }
}
