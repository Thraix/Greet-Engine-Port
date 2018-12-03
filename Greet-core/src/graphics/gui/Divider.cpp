#include "Divider.h"

namespace Greet
{
  Divider::Divider(const XMLObject& object, Component* parent)
    : Component(object,parent), vertical(false)
  {
    vertical = GUIUtils::GetBooleanFromXML(xmlObject,"vertical", false);

    if(vertical)
    {
      if(!xmlObject.HasProperty("width"))
        xmlObject.AddProperty("width", "1px");
      if(!xmlObject.HasProperty("height"))
        xmlObject.AddProperty("height", "100%");
    }
    else
    {
      if(!xmlObject.HasProperty("width"))
        xmlObject.AddProperty("width", "100%");
      if(!xmlObject.HasProperty("height"))
        xmlObject.AddProperty("height", "1px");
    }
    size = Vec2(CalculateWidth(), CalculateHeight());
  }
  void Divider::PreRender(GUIRenderer* renderer, const Vec2& translation) const
  {
    if(parent)
    {
      if(vertical)
        renderer->PushMatrix(Mat3::Translate(translation-Vec2(0,parent->GetPadding().LeftTop().h)));
      else
        renderer->PushMatrix(Mat3::Translate(translation-Vec2(parent->GetPadding().LeftTop().w,0)));
    }
    else
      renderer->PushMatrix(Mat3::Translate(translation));

    // Border around Component 
    if (xmlObject.HasProperty("borderColor"))
      renderer->SubmitRect(pos + Vec2(0,0), size, currentStyle->borderColor, false);

    // Component background
    if (xmlObject.HasProperty("backgroundColor"))
      renderer->SubmitRect(pos + currentStyle->border.LeftTop(), size-GetBorder().LeftTop()-GetBorder().RightBottom(), currentStyle->backgroundColor, false);

  }
  float Divider::CalculateWidth() const
  {
    if(parent && !vertical)
      return Component::CalculateWidth() + parent->GetPadding().GetWidth();
    return Component::CalculateWidth();
  }

  float Divider::CalculateHeight() const
  {
    if(parent && vertical)
      return Component::CalculateHeight() + parent->GetPadding().GetHeight();
    return Component::CalculateHeight();

  }
}
