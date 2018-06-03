#include "Content.h"

#include <graphics/gui/GUIUtils.h>
#include <utils/AABBUtils.h>

namespace Greet {

  Content::Content()
    : m_focused(NULL), parent(NULL), marginLeft(0), marginRight(0), marginTop(0), marginBottom(0), ySpacing(5), size(Vec2(100, 100)), xmlObject(XMLObject())
  {
    m_isFocusable = false;
  }

  Content::Content(const XMLObject& object, Content* parent)
    : parent(parent), m_focused(NULL), xmlObject(object.GetStrippedXMLObject())
  {
    size = Vec2(0, 0);
    marginTop = marginLeft = marginBottom = marginRight = 10;
    if (object.HasProperty("width"))
    {
      size.w = GUIUtils::CalcSize(object.GetProperty("width"), GetPotentialWidth());
    }
    if (object.HasProperty("height"))
    {
      size.h = GUIUtils::CalcSize(object.GetProperty("height"), GetPotentialHeight());
    }
    if (object.HasProperty("backgroundColor"))
    {
      backgroundColor = GUIUtils::GetColor(object.GetProperty("backgroundColor"));
    }
    if (object.HasProperty("marginTop"))
    {
      marginTop = GUIUtils::CalcSize(object.GetProperty("marginTop"), Window::GetHeight());
    }
    if (object.HasProperty("marginLeft"))
    {
      marginLeft = GUIUtils::CalcSize(object.GetProperty("marginLeft"), Window::GetWidth());
    }
    if (object.HasProperty("marginBottom"))
    {
      marginBottom = GUIUtils::CalcSize(object.GetProperty("marginBottom"), Window::GetHeight());
    }
    if (object.HasProperty("marginRight"))
    {
      marginRight = GUIUtils::CalcSize(object.GetProperty("marginRight"), Window::GetWidth());
    }
  }

  void Content::RenderHandle(GUIRenderer* renderer, const Vec2& position) const
  {
    if (xmlObject.HasProperty("backgroundColor"))
      renderer->SubmitRect(position, GetSize(), backgroundColor, false);
    Render(renderer, position);
    float yPos = marginTop;
    for (auto it = m_contents.begin(); it != m_contents.end(); ++it)
    {
      (*it)->RenderHandle(renderer, position + Vec2(marginLeft, yPos));
      yPos += (*it)->GetHeight() + ySpacing;
    }
  }

  void Content::Render(GUIRenderer* renderer, const Vec2& position) const
  {

  }

  void Content::UpdateHandle(float timeElapsed)
  {
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
      float yPos = marginTop;
      for (auto it = m_contents.begin(); it != m_contents.end(); ++it)
      {
        // Translate the mouse.
        Vec2 translatedPosContent = translatedPos - Vec2(marginLeft, yPos);
        bool mouseInside = (*it)->MousePressHandle(event, translatedPosContent);
        if (mouseInside)
        {
          // We always set the value since we want the "top" element, incase Contents overlap
          m_focused = *it;
        }
        yPos += (*it)->GetHeight() + ySpacing;
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
      float yPos = marginTop;
      for (auto it = m_contents.begin(); it != m_contents.end(); ++it)
      {
        if (m_focused == *it)
        {
          m_focused->MouseReleaseHandle(event, translatedPos - Vec2(marginLeft, yPos));
          if (m_focused->IsMouseInside(translatedPos - Vec2(marginLeft, yPos)))
          {
            //listener.OnMouseClicked(m_focused);
          }
        }
        yPos += (*it)->GetHeight() + ySpacing;
      }
    }
  }

  void Content::MouseMoveHandle(const MouseMovedEvent& event, const Vec2& translatedPos)
  {
    OnMouseMoved(event,translatedPos);
    if(m_focused != NULL)
    {
      // We loop since we need to calculate the yPos 
      float yPos = marginTop;
      for (auto it = m_contents.begin(); it != m_contents.end(); ++it)
      {
        if (m_focused == *it)
        {
          m_focused->MouseMoveHandle(event, translatedPos - Vec2(marginLeft, yPos));
        }
        yPos += (*it)->GetHeight() + ySpacing;
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
      return maxWidth + marginLeft + marginRight;
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
          return GUIUtils::CalcSize(h, Window::GetWidth());
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
      float height = marginTop + marginBottom;
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
        return size.w;
      }
      else
      {
        if (parent == NULL)
          return GUIUtils::CalcSize(w, Window::GetWidth());
        // No need to check for parent since the top parent doesn't have an xml object.
        return GUIUtils::CalcSize(w, parent->GetPotentialWidth());
      }
    }
    if (parent == NULL)
      return size.w;
    return parent->GetPotentialWidth();
  }

  float Content::GetPotentialHeight() const
  {
    if (xmlObject.HasProperty("height"))
    {
      const std::string& h = xmlObject.GetProperty("height");
      if (GUIUtils::IsStaticSize(h))
      {
        return size.h;
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
      return size.h;
    return parent->GetPotentialHeight();
  }

  void Content::SetMargins(float left, float right, float top, float bottom)
  {
    marginLeft = left;
    marginRight = right;
    marginTop = top;
    marginBottom = bottom;
  }
}
