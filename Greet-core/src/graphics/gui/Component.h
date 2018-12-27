#pragma once

#include <graphics/gui/Style.h>
#include <graphics/renderers/GUIRenderer.h>
#include <event/MouseEvent.h>
#include <event/KeyEvent.h>

namespace Greet
{

  class Component
  {
    public:
      enum class SizeType
      {
        WRAP, WEIGHT, NONE 
      };
      // Typedef callbacks
      typedef std::function<void(Component*)> OnClickCallback;
      typedef std::function<void(Component*)> OnPressCallback;
      typedef std::function<void(Component*)> OnReleaseCallback;

    protected:
      Style* currentStyle;

      Style normalStyle;
      Style hoverStyle;
      Style pressStyle;

      std::string name;

      XMLObject xmlObject;
      Component* parent;

      // Might get dirty?
      Vec2 pos;
      Vec2 size;

      Vec2 sizeType;

      SizeType widthSizeType;
      SizeType heightSizeType;


      bool m_isFocusable;
      bool isFocused;

      bool pressed;

      // Callbacks
      OnClickCallback onClickCallback;
      OnPressCallback onPressCallback;
      OnReleaseCallback onReleaseCallback;

    public:
      Component(const XMLObject& object, Component* parent);

      virtual void PostConstruction(){} 

      // Measurements that doesn't depend on the parent
      virtual void Measure();
      // Measurements that does depend on the parent
      void MeasureFill();
      virtual void MeasureFill(float parentEmptyWidth, float parentEmptyHeight, float parentTotalWeight, bool vertical);
      void Remeasure();

      // Size has updated
      virtual void OnMeasured(){}

      virtual Vec2 GetMeasureFillSize();
      virtual float GetMeasureTotalWeight();

      // Push translation to renderer
      virtual void PreRender(GUIRenderer* renderer, const Vec2& translation) const;

      // Render component
      virtual void RenderHandle(GUIRenderer* renderer) const;
      virtual void Render(GUIRenderer* renderer) const{}

      // Pop translation from renderer
      virtual void PostRender(GUIRenderer* renderer) const;

      virtual void UpdateHandle(float timeElapsed);
      virtual void Update(float timeElapsed){}

      // Returns the focused content
      virtual Component* OnMousePressed(const MousePressedEvent& event, const Vec2& translatedPos);
      // Returns the hovered content
      virtual Component* OnMouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos);

      // Callbacks
      void SetOnClickCallback(OnClickCallback callback);
      void SetOnPressCallback(OnPressCallback callback);
      void SetOnReleaseCallback(OnReleaseCallback callback);

      Vec2 GetPosition() const;
      Vec2 GetRealPosition() const;
      Vec2 GetSize() const;
      Component* GetParent() const;

      virtual Vec2 GetWrapSize() const;

      // Will return the stored value
      virtual float GetWidth() const;
      virtual float GetHeight() const;

      SizeType GetWidthSizeType() const;
      SizeType GetHeightSizeType() const;

      const Vec2& GetSizeType() const;

      const XMLObject& GetXMLObject() const;
      virtual bool IsMouseInside(const Vec2& parentMouse) const;
      virtual void SetPosition(const Vec2& pos);
      void SetSize(const Vec2& size);

      // These four will only be called if the component is focusable
      virtual void OnFocused();
      virtual void OnUnfocused();
      virtual void Focused(){}
      virtual void Unfocused(){}
      virtual void MouseEntered(){}
      virtual void MouseExited(){}

      // These functions will only be called if the component is focused
      virtual void MousePressed(const MousePressedEvent& event, const Vec2& translatedPos);
      virtual void MouseReleased(const MouseReleasedEvent& event, const Vec2& translatedPos);
      virtual void MouseMoved(const MouseMovedEvent& event, const Vec2& translatedPos){}
      virtual void KeyPressed(const KeyPressedEvent& event){}
      virtual void KeyReleased(const KeyReleasedEvent& event){}
      virtual void KeyTyped(const KeyTypedEvent& event){}

      Component* GetRootNode();
      Vec2 GetTotalPadding() const;
      const TLBR& GetMargin() const;
      const TLBR& GetPadding() const;
      const TLBR& GetBorder() const;
      Vec2 GetContentSize() const;
      const std::string& GetName() const;

      template <typename T>
        T* GetComponentByName(const std::string& name)
        {
          T* component = dynamic_cast<T*>(GetComponentByNameNoCast(name));
          if(component == nullptr)
            Log::Warning("Could not find Component with given type (name = ", name,")");
          return component;
        }

      virtual Component* GetComponentByNameNoCast(const std::string& name);

    protected:
      virtual void CallOnClickCallback();
      virtual void CallOnPressCallback();
      virtual void CallOnReleaseCallback();

  };
}
