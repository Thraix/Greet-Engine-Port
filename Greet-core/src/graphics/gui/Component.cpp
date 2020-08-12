#include "Component.h"

#include <graphics/gui/Container.h>
#include <input/InputDefines.h>
#include <utils/LogUtils.h>
#include <utils/AABBUtils.h>
#include <utils/UUID.h>

#include <functional>
#include <cstdlib>

namespace Greet
{

  REGISTER_COMPONENT_DEFINITION(Component);

  Component::Component(const std::string& name, Component* parent)
    : parent{parent}, size{}, isFocused{false}, isHovered{false}, pos{0,0}, pressed{false}, name{name}
  {
    AddStyleVariables({
      .colors=
      {
        {"backgroundColor", &backgroundColor},
        {"borderColor", &borderColor}
      },
      .tlbrs =
      {
        {"border", &border},
        {"margin", &margin},
        {"padding", &padding}
      },
      .floats =
      {
        {"radius", &backgroundRadius},
        {"borderRadius", &borderRadius}
      },
      .ints =
      {
        {"backgroundRoundedPrecision", &backgroundRoundedPrecision},
        {"borderRoundedPrecision", &borderRoundedPrecision}
      }
    });

    AddStyle("normal");
    AddStyle("hover", "normal");
    AddStyle("press", "normal");
    SetCurrentStyle("normal");
  }


  Component::Component(const XMLObject& xmlObject, Component* parent)
    : Component{GUIUtils::GetStringFromXML(xmlObject,"name", xmlObject.GetName() + "#" + LogUtils::DecToHex(UUID::GetInstance().GetUUID(),8)), parent}
  {
    size = GUIUtils::GetComponentSizeFromXML(xmlObject, "width", "height", {});
  }

  void Component::Measure(const Vec2& emptyParentSpace, const Vec2& percentageFill)
  {
    if(size.widthType == ComponentSize::Type::Pixels)
      size.size.w = size.value.w;
    else if(size.widthType == ComponentSize::Type::Weight)
      size.size.w = emptyParentSpace.x * percentageFill.x;
    else
      size.size.w = GetWrapWidth();

    if(size.heightType == ComponentSize::Type::Pixels)
      size.size.h = size.value.h;
    else if(size.heightType == ComponentSize::Type::Weight)
      size.size.h = emptyParentSpace.h * percentageFill.h;
    else
      size.size.h = GetWrapHeight();
    OnMeasured();
  }

  Vec2 Component::GetMeasureFillSize()
  {
    return GetContentSize();
  }

  float Component::GetMeasureTotalWeight()
  {
    return 1;
  }

  void Component::Remeasure()
  {
    remeasure = false;
    if(!parent)
    {
      Measure(guiScene->GetSize(), {1, 1});
    }
    else
    {
      GetRootNode()->Remeasure();
    }
  }

  // Push translation to renderer
  void Component::PreRender(GUIRenderer* renderer, const Vec2& translation) const
  {
    renderer->PushTranslation(translation);

    // Border around Component
    if(borderColor.a != 0.0)
      //renderer->SubmitRect(pos + Vec2(0,0), size, currentStyle->borderColor, false);
      renderer->DrawRoundedRect(pos+Vec2(0,0),size.size, borderColor, borderRadius, borderRoundedPrecision, false);

    // Component background
    if (backgroundColor.a != 0.0)
      renderer->DrawRoundedRect(pos + border.LeftTop(), size.size - GetBorder().LeftTop()-GetBorder().RightBottom(), backgroundColor, backgroundRadius, backgroundRoundedPrecision, false);
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
    renderer->PopTranslation();
  }

  void Component::UpdateHandle(float timeElapsed)
  {
    size.size = Vec2(GetWidth(), GetHeight());
    Update(timeElapsed);
  }

  void Component::OnEventHandler(Event& event, const Vec2& componentPos)
  {
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
      OnMousePressEventHandler(static_cast<MousePressEvent&>(event), componentPos);
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
      OnMouseMoveEventHandler(static_cast<MouseMoveEvent&>(event), componentPos);
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_RELEASE))
      OnMouseReleaseEventHandler(static_cast<MouseReleaseEvent&>(event), componentPos);
    else if(EVENT_IS_TYPE(event, EventType::KEY_PRESS))
      OnEvent(event, componentPos);
    else if(EVENT_IS_TYPE(event, EventType::KEY_RELEASE))
      OnEvent(event, componentPos);
    else if(EVENT_IS_TYPE(event, EventType::KEY_TYPE))
      OnEvent(event, componentPos);
    else
      OnEvent(event, componentPos);
  }

  void Component::OnMousePressEventHandler(MousePressEvent& event, const Vec2& componentPos)
  {
    if(!guiScene)
      Log::Error("GuiScene not initialized");
    if(!isFocused)
      guiScene->RequestFocus(this);
    if(event.GetButton() == GREET_MOUSE_1)
    {
      pressed = true;
      if(currentStyle == "hover")
        SetCurrentStyle("press");
      CallOnPressCallback();
    }
    OnEvent(event, componentPos);
  }

  void Component::OnMouseMoveEventHandler(MouseMoveEvent& event, const Vec2& componentPos)
  {
    if(IsMouseInside(event.GetPosition() - componentPos) || UsingMouse())
    {
      if(!isHovered)
      {
        if(currentStyle == "normal")
          SetCurrentStyle("hover");
        isHovered = true;
        MouseEntered();
      }
      OnEvent(event, componentPos);
    }
    else if(isHovered)
    {
      isHovered = false;
      MouseExited();
      if(currentStyle == "hover")
        SetCurrentStyle("normal");
    }
  }

  void Component::OnMouseReleaseEventHandler(MouseReleaseEvent& event, const Vec2& componentPos)
  {
    if(event.GetButton() == GREET_MOUSE_1)
    {
      if(pressed)
      {
        if(IsMouseInside(event.GetPosition() - componentPos))
        {
          if(currentStyle == "press")
            SetCurrentStyle("hover");
          CallOnClickCallback();
        }
        else if(currentStyle == "press")
          SetCurrentStyle("normal");
        CallOnReleaseCallback();
        OnEvent(event, componentPos);
        pressed = false;
      }
    }
  }

  void Component::SetOnClickCallback(OnClickCallback callback)
  {
    onClickCallback = callback;
  }

  void Component::SetOnPressCallback(OnPressCallback callback)
  {
    onPressCallback = callback;
  }

  void Component::SetOnReleaseCallback(OnReleaseCallback callback)
  {
    onReleaseCallback = callback;
  }

  void Component::CallOnClickCallback()
  {
    if(onClickCallback)
      onClickCallback(this);
  }

  void Component::CallOnPressCallback()
  {
    if(onPressCallback)
      onPressCallback(this);
  }

  void Component::CallOnReleaseCallback()
  {
    if(onReleaseCallback)
      onReleaseCallback(this);
  }

  void Component::OnFocused()
  {
    isFocused = true;
    Focused();
  }

  void Component::OnUnfocused()
  {
    isFocused = false;
    Unfocused();
  }

  bool Component::UsingMouse()
  {
    return pressed;
  }

  Vec2 Component::GetPosition() const
  {
    return pos;
  }

  Vec2 Component::GetRealPosition() const
  {
    return pos + GetMargin().LeftTop() + (parent ? parent->GetTotalPadding()+parent->GetRealPosition() : Vec2(0,0));
  }

  Component* Component::GetParent() const
  {
    return parent;
  }

  void Component::SetPosition(const Vec2& pos)
  {
    this->pos = pos;
  }

  Vec2 Component::GetSize() const
  {
    return size.size;
  }

  float Component::GetWidth() const
  {
    return size.size.w;
  }

  float Component::GetHeight() const
  {
    return size.size.h;
  }

  ComponentSize::Type Component::GetWidthSizeType() const
  {
    return size.widthType;
  }

  ComponentSize::Type Component::GetHeightSizeType() const
  {
    return size.heightType;
  }

  Component& Component::SetWidth(float width)
  {
    size.value.w = width;
    remeasure = true;
    return *this;
  }

  Component& Component::SetHeight(float height)
  {
    size.value.h = height;
    remeasure = true;
    return *this;
  }

  Component& Component::SetWidthSizeType(ComponentSize::Type width)
  {
    size.widthType = width;
    remeasure = true;
    return *this;
  }

  Component& Component::SetHeightSizeType(ComponentSize::Type height)
  {
    size.heightType = height;
    remeasure = true;
    return *this;
  }

  Component& Component::SetSize(float width, float height, ComponentSize::Type widthType, ComponentSize::Type heightType, bool remeasure)
  {
    size.value.w = width;
    size.value.h = height;
    size.widthType = widthType;
    size.heightType = heightType;
    if(remeasure)
      remeasure = true;
    return *this;
  }

  void Component::SetGUIScene(GUIScene* scene)
  {
    guiScene = scene;
  }

  const ComponentStyle& Component::GetStyle(const std::string& stylename) const
  {
    auto it = styles.find(stylename);
    if(it != styles.end())
      return it->second;
    Log::Error("Style does not exist within component: ", name, ", ", stylename);
    return styles.find("normal")->second;
  }

  ComponentStyle& Component::GetStyle(const std::string& stylename)
  {
    auto it = styles.find(stylename);
    if(it != styles.end())
      return it->second;
    Log::Error("Style does not exist within component: ", name, ", ", stylename);
    return styles.find("normal")->second;
  }

  Component& Component::AddStyle(const std::string& stylename, const std::string& inherit)
  {
    auto it = styles.find(stylename);
    if(it != styles.end())
    {
      Log::Error("Style already exist: ", stylename, " in ", name);
      return *this;
    }
    ComponentStyle* inheritStyle = nullptr;
    if(inherit != "")
      inheritStyle = &GetStyle(inherit);
    it = styles.emplace(stylename, ComponentStyle{styleVariables, inheritStyle}).first;
    return *this;
  }

  Component& Component::AddStyleVariables(const StylingVariables& variables)
  {
    styleVariables.Merge(variables);
    for(auto&& style : styles)
    {
      style.second.AddVariables(variables);
    }
    return *this;
  }

  void Component::LoadStyles(const XMLObject& xmlObject)
  {
    for(auto&& style : styles)
    {
      style.second.Load(style.first, xmlObject);
    }
  }

  Component& Component::LoadStyle(const std::string& stylename, const Styling& styling)
  {
    auto it = styles.find(stylename);
    if(it == styles.end())
    {
      Log::Error("Style does not exist within component: ", name, ", ", stylename);
      return *this;
    }
    it->second.Load(styling);
    return *this;
  }

  void Component::SetCurrentStyle(const std::string& stylename)
  {
    auto it = styles.find(stylename);
    if(it != styles.end())
    {
      currentStyle = stylename;
      it->second.SetStyling();
      return;
    }
    Log::Error("Style does not exist within component: ", name, ", ", stylename);
  }

  const Vec2& Component::GetSizeValue() const
  {
    return size.value;
  }

  Component* Component::GetComponentByNameNoCast(const std::string& name)
  {
    if(name == this->name)
      return this;
    return nullptr;
  }

  bool Component::IsMouseInside(const Vec2& translatedPos) const
  {
    return AABBUtils::PointInsideBox(translatedPos, Vec2{0,0}, size.size);
  }

  Component* Component::GetRootNode()
  {
    Component* par = this;
    while(par->GetParent())
      par = par->GetParent();
    return par;
  }

  uint Component::GetComponentDepth() const
  {
    uint depth = 1;
    const Component* par = this;
    while(par->GetParent())
    {
      depth++;
      par = par->GetParent();
    }
    return depth;
  }

  Vec2 Component::GetTotalPadding() const
  {
    return GetPadding().LeftTop() + GetBorder().LeftTop();
  }

  Vec2 Component::GetContentSize() const
  {
    return size.size - GetPadding().GetSize() - GetBorder().GetSize();
  }

  const std::string& Component::GetName() const
  {
    return name;
  }

  const TLBR& Component::GetMargin() const
  {
    return margin;
  }

  const TLBR& Component::GetPadding() const
  {
    return padding;
  }

  const TLBR& Component::GetBorder() const
  {
    return border;
  }
}
