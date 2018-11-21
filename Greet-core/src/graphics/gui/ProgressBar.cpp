#include "ProgressBar.h"

#include <stdlib.h>

namespace Greet
{
  ProgressBar::ProgressBar(const XMLObject& object, Component* parent)
    : Component(object,parent), min(0), max(1), value(0), valueReference(nullptr), vertical(false), reverse(false)
  {
    if(object.HasProperty("minValue"))
      min = atof(object.GetProperty("minValue").c_str()); 
    if(object.HasProperty("maxValue"))
      max = atof(object.GetProperty("maxValue").c_str()); 
    if(object.HasProperty("vertical"))
      vertical = GUIUtils::GetBoolean(object.GetProperty("vertical")); 
    if(object.HasProperty("reverseProgress"))
      reverse = GUIUtils::GetBoolean(object.GetProperty("reverseProgress")); 
  }

  void ProgressBar::Render(GUIRenderer* renderer) const
  {
    Vec2 localSize = GetContentSize();
    float percentage = (value-min)/(max-min);
    if(vertical)
      localSize.h *= percentage;
    else
      localSize.w *= percentage;

    Vec2 progressPos = pos + GetBorder().LeftTop() + GetPadding().LeftTop(); 

    if(reverse)
      renderer->SubmitRect(progressPos + (GetContentSize() - localSize), localSize, Vec4(0,1,0,1), false);
    else
      renderer->SubmitRect(progressPos,  localSize, Vec4(0,1,0,1), false);
  }

  void ProgressBar::Update(float timeElapsed)
  {
    if(valueReference != nullptr)
      value = *valueReference;
    Math::Clamp(&value,min,max);
  }

  void ProgressBar::AttachValueReference(float* reference)
  {
    valueReference = reference; 
  }

  float ProgressBar::GetValue() const
  {
    return value;
  }

  float* ProgressBar::GetValueReference() const
  {
    return valueReference;
  }

  void ProgressBar::SetValue(float value)
  {
    if(!valueReference)
      Log::Warning("Setting value to ProgressBar when value reference exists");
    this->value = value;
    Math::Clamp(&this->value,min,max);
  
  }
}