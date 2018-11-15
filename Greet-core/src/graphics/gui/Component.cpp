#include "Component.h"

#include <utils/AABBUtils.h>
#include <graphics/gui/Content.h>

namespace Greet
{
  Component::Component()
    : xmlObject(XMLObject()), parent(nullptr)
  {

  }

  Component::Component(const XMLObject& object, Component* parent)
    : xmlObject(object.GetStrippedXMLObject()), parent(parent)
  {
    size = Vec2(100, 100);
    Style temp;
    currentStyle = &temp;
    if (object.HasProperty("width"))
    {
      size.w = GUIUtils::CalcSize(object.GetProperty("width"), GetPotentialWidth());
    }
    if (object.HasProperty("height"))
    {
      size.h = GUIUtils::CalcSize(object.GetProperty("height"), GetPotentialHeight());
    }

    normalStyle.Load("",*this);
    hoverStyle.Load("hover_",*this,&normalStyle);
    pressStyle.Load("press_",*this,&normalStyle);
    currentStyle = &normalStyle;
  }

  // Push translation to renderer
  void Component::PreRender(GUIRenderer* renderer, const Vec2& translation) const
  {
    renderer->PushMatrix(Mat3::Translate(translation));

    // Border around Component 
    if (xmlObject.HasProperty("borderColor"))
      renderer->SubmitRect(Vec2(0,0), size + currentStyle->border.LeftTop() + currentStyle->border.RightBottom(), currentStyle->borderColor, false);

    // Component background
    if (xmlObject.HasProperty("backgroundColor"))
      renderer->SubmitRect(currentStyle->border.LeftTop(), size, currentStyle->backgroundColor, false);
  }

  // Render component
  void Component::RenderHandle(GUIRenderer* renderer) const
  {
    // Render rest of component
    Render(renderer);
  }

  // Pop translation from renderer
  void Component::PostRender(GUIRenderer* renderer) const
  {
    renderer->PopMatrix();
  }

  void Component::UpdateHandle(float timeElapsed)
  {
    size = Vec2(GetWidth(), GetHeight());
    Update(timeElapsed);
  }

  Vec2 Component::GetPosition() const
  {
    return pos;
  }

  Vec2 Component::GetSize() const
  {
    return size;
  }

  void Component::SetSize(const Vec2& size)
  {
    this->size = size;
  }

  float Component::GetWidth() const
  {
    if (xmlObject.HasProperty("width"))
    {
      const std::string& w = xmlObject.GetProperty("width");
      if (!GUIUtils::IsStaticSize(w))
      {
        if (parent == NULL)
          return GUIUtils::CalcSize(w, Window::GetWidth());
        return GUIUtils::CalcSize(w, parent->GetPotentialWidth());
      }
      return size.w;
    }
    else if (parent == NULL)
    {
      return size.w;
    }
#if 0
    else
    {
      float maxWidth = 0;
      for (auto it = m_components.begin(); it != m_contents.end(); ++it)
      {
        maxWidth = Math::Max((*it)->GetWidth(), maxWidth);
      }
      return maxWidth + currentStyle->margin.left + currentStyle->margin.right;
    }
#endif
    return size.w;
  }

  float Component::GetHeight() const
  {
    if (xmlObject.HasProperty("height"))
    {
      const std::string& h = xmlObject.GetProperty("height");
      if (!GUIUtils::IsStaticSize(h))
      {
        if (parent == NULL)
          return GUIUtils::CalcSize(h, Window::GetHeight());
        return GUIUtils::CalcSize(h, parent->GetPotentialHeight());
      }
      return size.h;
    }
    else if (parent == NULL)
    {
      return size.h;
    }
#if 0
    else
    {
      float height = currentStyle->margin.top + currentStyle->margin.bottom;
      for (auto it = m_contents.begin(); it != m_contents.end(); ++it)
      {
        if (it != m_contents.begin())
          height += ySpacing;
        height += (*it)->GetHeight();
      }
      return height;
    }
#endif
    return size.h;
  }

  float Component::GetPotentialWidth() const
  {
    if (xmlObject.HasProperty("width"))
    {
      const std::string& w = xmlObject.GetProperty("width");
      if (GUIUtils::IsStaticSize(w))
      {
        return size.w - currentStyle->padding.right - currentStyle->padding.left - currentStyle->border.right - currentStyle->border.left;
      }
      else
      {
        if (parent == NULL)
          return GUIUtils::CalcSize(w, Window::GetWidth());
        // No need to check for parent since the top parent doesn't have an xml object.
        return GUIUtils::CalcSize(w, parent->GetPotentialWidth()-currentStyle->margin.left-currentStyle->margin.right);
      }
    }
    if (parent == NULL)
      return size.w - currentStyle->padding.right - currentStyle->padding.left - currentStyle->border.right - currentStyle->border.left;
    return parent->GetPotentialWidth();
  }

  float Component::GetPotentialHeight() const
  {
    if (xmlObject.HasProperty("height"))
    {
      const std::string& h = xmlObject.GetProperty("height");
      if (GUIUtils::IsStaticSize(h))
      {
        return size.h - currentStyle->margin.top - currentStyle->margin.bottom;
      }
      else
      {
        if (parent == NULL)
          return GUIUtils::CalcSize(h, Window::GetHeight());
        // No need to check for parent since the top parent doesn't have an xml object.
        return GUIUtils::CalcSize(h, parent->GetPotentialHeight());
      }
    }
    if (parent == NULL)
      return size.h - currentStyle->margin.top - currentStyle->margin.bottom;
    return parent->GetPotentialHeight();
  }
  const XMLObject& Component::GetXMLObject() const
  {
    return xmlObject;
  }

  bool Component::IsMouseInside(const Vec2& parentMouse) const
  {
    return AABBUtils::PointInsideBox(parentMouse, Vec2(0, 0), size);
  }
}
