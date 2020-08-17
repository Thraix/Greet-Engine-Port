#include "Slider.h"

#include <input/InputDefines.h>
#include <math/Maths.h>

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(Slider);

  Slider::Slider(const std::string& name, Component* parent)
    : Component{name, parent}
  {
    AddStyleVariables(StylingVariables{.bools = {{"vertical", &vertical}}});
    Styling normal
    {
      .colors = {
        {"backgroundColor", {1,1,1,1}},
        {"borderColor", {0,0,0,1}},
      },
      .tlbrs = {{"border", {2,2,2,2}}}
    };

    sliderComponent = new Component(name+"#SliderComponent", this);
    sliderComponent->SetSize(7,1,GUISize::Type::Pixels, GUISize::Type::Weight)
      .LoadStyle("normal", normal);

    minValue = 0;
    maxValue = 100;
    stepSize = 0;
    // Bit of a hack to make the initial value correct
    minPos = 0;
    maxPos = 1;

    SetValue((maxValue-minValue)/2.0f);
  }

  Slider::Slider(const XMLObject& xmlObject, Component* parent)
    : Component(xmlObject, parent)
  {
    AddStyleVariables(StylingVariables{.bools = {{"vertical", &vertical}}});

    minValue = GUIUtils::GetFloatFromXML(xmlObject, "minValue", 0.0f);
    maxValue = GUIUtils::GetFloatFromXML(xmlObject, "maxValue", 100.0f);
    stepSize = GUIUtils::GetFloatFromXML(xmlObject, "stepSize", 0.0f);

    if(maxValue - minValue < 0)
    {
      Log::Warning("Min value in slider is greater than max value");
      std::swap(minValue,maxValue);
    }

    //if((maxValue - minValue) / stepSize > 1.0001)
    //  Log::Warning("Step size in Slider doesn't divide max - min evenly");


    if(stepSize < 0)
    {
      stepSize = -stepSize;
      Log::Warning("Step size in Slider is negative");
    }

    if(stepSize > 0)
      snapSlider = true;


    // Bit of a hack to make the initial value correct
    minPos = 0;
    maxPos = 1;

    if(xmlObject.GetObjectCount() > 0)
      sliderComponent = ComponentFactory::GetComponent(xmlObject.GetObject(0), this);
    else
    {
      Styling normal
      {
        .colors = {
          {"backgroundColor", {1,1,1,1}},
          {"borderColor", {0,0,0,1}},
        },
        .tlbrs = {{"border", {2,2,2,2}}}
      };

      clampSlider = GUIUtils::GetBooleanFromXML(xmlObject, "indicatorInside", false);
      sliderComponent = new Component(name+"#SliderComponent", this);
      if(vertical)
        sliderComponent->SetSize(1,7,GUISize::Type::Weight, GUISize::Type::Pixels);
      else
        sliderComponent->SetSize(7,1,GUISize::Type::Pixels, GUISize::Type::Weight);
      sliderComponent->LoadStyle("normal", normal);
    }

    // Default defaultValue should be in the middle
    SetValue(GUIUtils::GetFloatFromXML(xmlObject, "defaultValue", (maxValue-minValue)/2.0f));
  }

  void Slider::Measure(const Vec2& emptyParentSpace, const Vec2& percentageFill)
  {
    Component::Measure(emptyParentSpace, percentageFill);
    sliderComponent->Measure(GetContentSize(),{1,1});
  }

  void Slider::OnMeasured()
  {
    if(vertical && height.size == 0)
      return;
    if(!vertical && width.size == 0)
      return;
    float value = GetValue();

    // vi will be the y value of the sizes if vertical and x value if not
    float& sliderDirSize =vertical ? height.size : width.size;
    uint vi = vertical ? 1 : 0;

    if(clampSlider)
    {
      minPos = sliderComponent->GetSize()[vi]/2;
      maxPos = sliderDirSize - GetBorder().GetSize()[vi]-minPos;
    }
    else
    {
      minPos = 0;
      maxPos = sliderDirSize;
    }

    SetValue(value);
  }

  void Slider::Render(GUIRenderer* renderer) const
  {
    Vec2 sliderOffset = pos + GetTotalPadding() + sliderComponent->GetMargin().LeftTop() - sliderComponent->GetSize()/2;
    if(vertical)
      sliderComponent->PreRender(renderer,  sliderOffset + Vec2{GetContentSize().w/2, sliderPos});
    else
      sliderComponent->PreRender(renderer,  sliderOffset + Vec2{sliderPos, GetContentSize().h/2});
    sliderComponent->RenderHandle(renderer);
    sliderComponent->PostRender(renderer);
  }

  void Slider::OnEvent(Event& event, const Vec2& componentPos)
  {
    if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      if(e.GetButton() == GREET_MOUSE_1 && pressed)
      {
        Vec2 translatedPos = e.GetPosition() - componentPos;
        float oldValue = GetSliderValueFromPos(sliderPos);
        if(vertical)
          SetValue(GetSliderValueFromPos(translatedPos.y));
        else
          SetValue(GetSliderValueFromPos(translatedPos.x));
        float newValue = GetSliderValueFromPos(sliderPos);
        if(oldValue != newValue)
          CallOnValueChangeCallback(oldValue, newValue);
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
      if(pressed)
      {
        Vec2 translatedPos = e.GetPosition() - componentPos;
        float oldValue = GetSliderValueFromPos(sliderPos);
        if(vertical)
          SetValue(GetSliderValueFromPos(translatedPos.y));
        else
          SetValue(GetSliderValueFromPos(translatedPos.x));
        float newValue = GetSliderValueFromPos(sliderPos);
        if(oldValue != newValue)
          CallOnValueChangeCallback(oldValue, newValue);
      }
    }

  }

  void Slider::SetOnValueChangeCallback(OnValueChangeCallback callback)
  {
    onValueChangeCallback = callback;
  }

  void Slider::CallOnValueChangeCallback(float oldValue, float newValue)
  {
    if(onValueChangeCallback)
      onValueChangeCallback(this, oldValue, newValue);
  }

  Slider& Slider::SetVertical(bool vertical)
  {
    this->vertical = vertical;
    return *this;
  }

  Component* Slider::GetSliderComponent()
  {
    return sliderComponent;
  }

  float Slider::GetValue() const
  {
    return GetSliderValueFromPos(sliderPos);
  }

  void Slider::SetValue(float value)
  {
    Math::Clamp(&value, minValue, maxValue);
    if(snapSlider)
      value = GetSnappedSlider(value);
    sliderPos = GetSliderPosFromValue(value);
  }

  float Slider::GetSnappedSlider(float sliderValue) const
  {
    return Math::RoundClose(sliderValue,stepSize);
  }

  float Slider::GetSliderValueFromPos(float pos) const
  {
    return ((pos - minPos) / (maxPos - minPos)) * (maxValue-minValue) + minValue;
  }

  float Slider::GetSliderPosFromValue(float value) const
  {
    return (value - minValue) / (maxValue - minValue) * (maxPos-minPos) + minPos;
  }
}
