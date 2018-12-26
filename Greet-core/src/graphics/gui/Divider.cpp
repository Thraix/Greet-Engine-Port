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
        xmlObject.AddProperty("height", "fill_parent");
    }
    else
    {
      if(!xmlObject.HasProperty("width"))
        xmlObject.AddProperty("width", "fill_parent");
      if(!xmlObject.HasProperty("height"))
        xmlObject.AddProperty("height", "1px");
    }
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

  void Divider::OnMeasured() 
  {
    if(parent)
    {
      if(vertical)
      {
        size.h += parent->GetPadding().GetHeight();
      }
      else
      {
        size.w += parent->GetPadding().GetWidth();
      }
    }
  }

  Vec2 Divider::GetWrapSize() const
  {
    if(parent && !vertical)
      return Vec2(100, 1);
    return Vec2(1, 100);
  }
}
