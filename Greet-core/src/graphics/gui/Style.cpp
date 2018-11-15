#include "Style.h"

#include <graphics/gui/Content.h>

namespace Greet
{
  void Style::Load(const std::string& prefix, const Content& content, Style const * inherit)
  {
    margin.Load(prefix+"margin", content.GetXMLObject(), content.GetPotentialWidth(), content.GetPotentialHeight());
    padding.Load(prefix+"padding", content.GetXMLObject(), content.GetPotentialWidth(), content.GetPotentialHeight());
    border.Load(prefix+"border", content.GetXMLObject(), content.GetPotentialWidth(), content.GetPotentialHeight());
    if (content.GetXMLObject().HasProperty("backgroundColor"))
    {
      backgroundColor = GUIUtils::GetColor(content.GetXMLObject().GetProperty("backgroundColor"));
    }
    if (content.GetXMLObject().HasProperty("borderColor"))
    {
      borderColor = GUIUtils::GetColor(content.GetXMLObject().GetProperty("borderColor"));
    }
  }
}
