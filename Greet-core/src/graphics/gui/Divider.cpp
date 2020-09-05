#include "Divider.h"

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(Divider);

  Divider::Divider(const XMLObject& xmlObject, Component* parent)
    : Component(xmlObject,parent), vertical(false)
  {
    AddStyleVariables(StylingVariables{
        .bools = {{"vertical", &vertical}}
        });
    LoadStyles(xmlObject);
  }

  void Divider::PreRender(GUIRenderer* renderer, const Vec2& translation) const
  {
    Vec2 trans = translation;
    if(parent)
    {
      if(vertical)
        trans.y -= parent->GetPadding().top;
      else
        trans.x -= parent->GetPadding().left;
    }
    Component::PreRender(renderer, trans);
  }

  void Divider::OnMeasured()
  {
    if(parent)
    {
      if(vertical)
        height.size += parent->GetPadding().GetHeight();
      else
        width.size += parent->GetPadding().GetWidth();
    }
  }

  float Divider::GetWrapWidth() const
  {
    if(!vertical)
      return 100;
    return 1;
  }

  float Divider::GetWrapHeight() const
  {
    if(vertical)
      return 100;
    return 1;
  }
}
