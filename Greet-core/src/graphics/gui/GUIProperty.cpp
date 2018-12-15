#if 0
#include "GUIProperty.h"

namespace Greet
{
  float GUIProperty::GetPropertyFloat(const Property& property, const XMLObject& xmlObject, Content* content, float defaultValue)
  {
    if(xmlObject.HasProperty(property))
    {
      switch(property)
      {
        case WIDTH: case MARGIN_LEFT: case MARGIN_RIGHT: case BORDER_LEFT: case BORDER_RIGHT: 
          return GUIUtils::CalcSize(xmlObject.GetProperty(property), content->GetPotentialWidth());
        case HEIGHT: case MARGIN_TOP: case MARGIN_BOTTOM: case BORDER_TOP: case BORDER_BOTTOM: 
          return GUIUtils::CalcSize(xmlObject.GetProperty(property), content->GetPotentialHeight());
        default:
          Log::Warning("Invalid property, not float: ", property);
          return defaultValue;
      }
    }
    return defaultValue;
  }
  Vec4 GUIProperty::GetPropertyVec4(const Property& property, Content* content, const Vec4& defaultValue)
  {
    if(xmlObject.HasProperty(property))
    {
      switch(property)
      {
        case BACKGROUND_COLOR: case BORDER_COLOR:
          return GUIUtils::GetColor(xmlObject.GetProperty(property));
        default:
          Log::Warning("Invalid property, not Vec4: ", property);
          return defaultValue;
      }
    }
    return defaultValue;
  }
  std::string GUIProperty::GetPropertyName(const Property& property)
  {
    switch(property)
    {

    }
  }
}
#endif
