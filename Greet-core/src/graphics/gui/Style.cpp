#include "Style.h"

#include <graphics/gui/Component.h>

namespace Greet
{
  void Style::Load(const std::string& prefix, const Component& component, Style const * inherit)
  {
    margin.Load(prefix+"margin", component.GetXMLObject(), component.GetPotentialWidth(), component.GetPotentialHeight());
    padding.Load(prefix+"padding", component.GetXMLObject(), component.GetPotentialWidth(), component.GetPotentialHeight());
    border.Load(prefix+"border", component.GetXMLObject(), component.GetPotentialWidth(), component.GetPotentialHeight());
    if (component.GetXMLObject().HasProperty("backgroundColor"))
    {
      backgroundColor = GUIUtils::GetColor(component.GetXMLObject().GetProperty("backgroundColor"));
    }
    if (component.GetXMLObject().HasProperty("borderColor"))
    {
      borderColor = GUIUtils::GetColor(component.GetXMLObject().GetProperty("borderColor"));
    }
  }
}
