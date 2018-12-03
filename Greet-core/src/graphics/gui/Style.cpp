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
    backgroundColor = GUIUtils::GetColorFromXML(component.GetXMLObject(), prefix+"backgroundColor",Vec4(0,0,0,1));
    borderColor = GUIUtils::GetColorFromXML(component.GetXMLObject(), prefix+"borderColor",Vec4(0,0,0,1));
  }
}
