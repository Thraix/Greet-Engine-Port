#include "Content.h"

#include <graphics/gui/GUIUtils.h>
#include <utils/AABBUtils.h>

namespace Greet {

  Content::Content()
    : m_focused(NULL), parent(NULL), ySpacing(10), size(Vec2(100, 100)), xmlObject(XMLObject())
  {
    m_isFocusable = false;
  }

  Content::Content(const XMLObject& object, Content* parent)
    : parent(parent), m_focused(NULL), xmlObject(object.GetStrippedXMLObject()) 
  {
    size = Vec2(100, 100);
    ySpacing = 10;
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
    if (object.HasProperty("spacing"))
    {
      ySpacing = GUIUtils::CalcSize(object.GetProperty("spacing"), GetPotentialHeight());
    }
    normalStyle.Load("",*this);
    hoverStyle.Load("hover_",*this,&normalStyle);
    pressStyle.Load("press_",*this,&normalStyle);
    currentStyle = &normalStyle;
  }

  void Content::RenderHandle(GUIRenderer* renderer, const Vec2& position) const
  {
    // Border around Content 
    if (xmlObject.HasProperty("borderColor"))
      renderer->SubmitRect(position, size + Vec2(currentStyle->margin.left, currentStyle->margin.top), currentStyle->borderColor, false);
    // Content background
    if (xmlObject.HasProperty("backgroundColor"))
      renderer->SubmitRect(position + Vec2(currentStyle->border.left, currentStyle->border.top), size - Vec2(currentStyle->border.left + currentStyle->border.right, currentStyle->border.top + currentStyle->border.bottom), currentStyle->backgroundColor, false);
    Render(renderer, position);
    Vec2 childOffset(position.x + currentStyle->border.left + currentStyle->padding.left, position.y +currentStyle->padding.top + currentStyle->border.top);
    for (auto it = m_contents.begin(); it != m_contents.end(); ++it)
    {
      (*it)->RenderHandle(renderer, childOffset+ Vec2((*it)->currentStyle->margin.left, (*it)->currentStyle->margin.top));
      childOffset += GetChildOffset(*it);
    }
  }

  void Content::Render(GUIRenderer* renderer, const Vec2& position) const
  {

  }

  void Content::UpdateHandle(float timeElapsed)
  {
    size = GetSize();
    Update(timeElapsed);
    for (auto it = m_contents.begin(); it != m_contents.end(); ++it)
    {
      (*it)->UpdateHandle(timeElapsed);
    }
  }

  void Content::Update(float timeElapsed)
  {

  }

  void Content::AddContent(Content* content)
  {
    if (content == NULL)
    {
      Log::Warning("Cannot add NULL to content");
      return;
    }
    m_contents.push_back(content);
  }

  Content* Content::RemoveContent(uint index)
  {
    if (index >= m_contents.size())
    {
      Log::Warning("Index out of bound.");
      return NULL;
    }
    auto it = m_contents.begin() + index;
    m_contents.erase(it);
    return *it;
  }

  Content* Content::RemoveContent(Content* content)
  {
    for (auto it = m_contents.begin(); it != m_contents.end();++it)
    {
      if (*it == content)
      {
        m_contents.erase(it);
        return content;
      }
    }
    Log::Warning("Couldn't find content");
    return NULL;
  }

  Content* Content::GetContent(uint index)
  {
    if (index >= m_contents.size())
    {
      Log::Warning("Index out of bound.");
      return NULL;
    }
    return *(m_contents.begin() + index);
  }


  bool Content::MousePressHandle(const MousePressedEvent& event, const Vec2& translatedPos)
  {
    if(IsMouseInside(translatedPos)){
      OnMousePressed(event,translatedPos);
      Content* lastFocus = m_focused;
      m_focused = NULL;
      Vec2 childOffset(translatedPos.x - currentStyle->border.left - currentStyle->padding.left, translatedPos.y -currentStyle->padding.top - currentStyle->border.top);
      for (auto it = m_contents.begin(); it != m_contents.end(); ++it)
      {
        // Translate the mouse.
        bool mouseInside = (*it)->MousePressHandle(event, childOffset - Vec2((*it)->currentStyle->margin.left, (*it)->currentStyle->margin.top));
        if (mouseInside)
        {
          // We always set the value since we want the "top" element, incase Contents overlap
          m_focused = *it;
        }
        childOffset -= GetChildOffset(*it);
      }
      if (m_focused == NULL)
      {
        if(lastFocus != NULL)
          lastFocus->OnUnfocused();
      }
      else if(lastFocus != m_focused)
      {
        if(lastFocus != NULL)
          lastFocus->OnUnfocused();
        m_focused->OnFocused();
      }
      return true;
    }
    return false;
  }

  void Content::MouseReleaseHandle(const MouseReleasedEvent& event, const Vec2& translatedPos)
  {
    OnMouseReleased(event,translatedPos);
    if (m_focused != NULL)
    {
      // Calculate the focused yPos
      Vec2 childOffset(translatedPos.x - currentStyle->border.left - currentStyle->padding.left, translatedPos.y -currentStyle->padding.top - currentStyle->border.top);
      for (auto it = m_contents.begin(); it != m_contents.end(); ++it)
      {
        if (m_focused == *it)
        {
          m_focused->MouseReleaseHandle(event, childOffset - Vec2((*it)->currentStyle->margin.left, (*it)->currentStyle->margin.top));
          //if (m_focused->IsMouseInside(translatedPos - Vec2(margin.left, yPos)))
          {
            //listener.OnMouseClicked(m_focused);
          }
        }
        childOffset -= GetChildOffset(*it);
      }
    }
  }

  void Content::MouseMoveHandle(const MouseMovedEvent& event, const Vec2& translatedPos)
  {
    OnMouseMoved(event,translatedPos);
    if(m_focused != NULL)
    {
      // We loop since we need to calculate the yPos 
      Vec2 childOffset(translatedPos.x - currentStyle->border.left - currentStyle->padding.left, translatedPos.y -currentStyle->padding.top - currentStyle->border.top);
      for (auto it = m_contents.begin(); it != m_contents.end(); ++it)
      {
        if (m_focused == *it)
        {
          m_focused->MouseMoveHandle(event, childOffset - Vec2((*it)->currentStyle->margin.left, (*it)->currentStyle->margin.top));
        }
        childOffset -= GetChildOffset(*it);
      }
    }
  }

  void Content::KeyPressedHandle(const KeyPressedEvent& event)
  {
    OnKeyPressed(event);
    if (m_focused != NULL)
    {
      m_focused->OnKeyPressed(event);
    }
  }

  void Content::KeyReleasedHandle(const KeyReleasedEvent& event)
  {
    OnKeyReleased(event);
    if (m_focused != NULL)
    {
      m_focused->OnKeyReleased(event);
    }
  }

  bool Content::IsMouseInside(const Vec2& mousePos) const
  {
    return AABBUtils::PointInsideBox(mousePos, Vec2(0, 0), GetSize());
  }

  void Content::OnFocused()
  {

  }

  void Content::OnUnfocused()
  {
    // Unfocus the focused content.
    if (m_focused != NULL)
    {
      m_focused->OnUnfocused();
    }
  }

  void Content::SetSpacing(float spacing)
  {
    ySpacing = spacing;
  }

  float Content::GetSpacing() const
  {
    return ySpacing;
  }

  Vec2 Content::GetSize() const
  {
    return Vec2(GetWidth(), GetHeight());
  }

  void Content::SetSize(const Vec2& size)
  {
    this->size = size;
  }

  float Content::GetWidth() const
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
    else
    {
      float maxWidth = 0;
      for (auto it = m_contents.begin(); it != m_contents.end(); ++it)
      {
        maxWidth = Math::Max((*it)->GetWidth(), maxWidth);
      }
      return maxWidth + currentStyle->margin.left + currentStyle->margin.right;
    }
  }

  float Content::GetHeight() const
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
  }

  float Content::GetPotentialWidth() const
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

  float Content::GetPotentialHeight() const
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

  const XMLObject& Content::GetXMLObject() const
  {
    return xmlObject;
  }

  void Content::SetMargins(float left, float right, float top, float bottom)
  {
    currentStyle->margin.left = left;
    currentStyle->margin.right = right;
    currentStyle->margin.top = top;
    currentStyle->margin.bottom = bottom;
  }

  Vec2 Content::GetChildOffset(const Content* child) const
  {
    return Vec2(0,ySpacing + child->GetHeight() + child->currentStyle->margin.top + child->currentStyle->margin.bottom);
  }
}
