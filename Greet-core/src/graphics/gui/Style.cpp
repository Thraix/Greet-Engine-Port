#include "Style.h"

#include <graphics/gui/Component.h>
#include <graphics/gui/GLayer.h>

namespace Greet
{
  void Style::Load(const std::string& prefix, const Component& component, Style const * inherit)
  {
    Component* parent = component.GetParent();
    float w,h;
    if(parent)
    {
      w = parent->GetWidth(); 
      h = parent->GetHeight(); 
    }
    else
    {
      w = GLayer::GetWidth(); 
      h = GLayer::GetHeight();
    }
    margin.Load(prefix+"margin", component.GetXMLObject(), w, h);
    padding.Load(prefix+"padding", component.GetXMLObject(), w, h);
    border.Load(prefix+"border", component.GetXMLObject(), w, h);
    if (component.GetXMLObject().HasProperty(prefix+"backgroundColor"))
    {
      backgroundColor = GUIUtils::GetColor(component.GetXMLObject().GetProperty(prefix+"backgroundColor"));
    }
    if (component.GetXMLObject().HasProperty(prefix+"borderColor"))
    {
      borderColor = GUIUtils::GetColor(component.GetXMLObject().GetProperty(prefix+"borderColor"));
    }
  }
}
