#include "Style.h"

#include <graphics/gui/Component.h>
#include <graphics/gui/GLayer.h>

namespace Greet
{
  void Style::Load(const std::string& prefix, const XMLObject& object, Style const * inherit)
  {
    margin.Load(prefix+"margin", object);
    padding.Load(prefix+"padding", object);
    border.Load(prefix+"border", object);

    radius = GUIUtils::GetFloatFromXML(object, prefix+"radius",0);
    borderRadius = GUIUtils::GetFloatFromXML(object, prefix+"borderRadius",radius);
    roundedPrecision = GUIUtils::GetIntFromXML(object, prefix+"roundedPrecision", 3);

    if(object.HasProperty(prefix+"backgroundColor"))
      hasBackgroundColor = true;
    if(object.HasProperty(prefix+"borderColor"))
      hasBorderColor = true;

    backgroundColor = GUIUtils::GetColorFromXML(object, prefix+"backgroundColor",Vec4(0,0,0,1));
    borderColor = GUIUtils::GetColorFromXML(object, prefix+"borderColor",Vec4(0,0,0,1));
  }
}
