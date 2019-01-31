#pragma once

#include <graphics/gui/Style.h>
#include <graphics/renderers/GUIRenderer.h>
#include <event/MouseEvent.h>
#include <event/KeyEvent.h>
#include <graphics/gui/ComponentFactory.h>
#include <graphics/gui/GUIScene.h>

namespace Greet
{

  class Component
  {
    public:
      friend class GUIScene;
    private:
      REGISTER_COMPONENT_DECLARATION(Component);
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

      GUIScene* guiScene;
      Component* parent;

      // Might get dirty?
      Vec2 pos;
      Vec2 size;

      Vec2 sizeType;

      SizeType widthSizeType;
      SizeType heightSizeType;


      bool m_isFocusable;
      bool isFocused;
      bool isHovered;
      bool pressed;

      bool isChildFocused;
      bool childChangedFocus;

      // Callbacks
      OnClickCallback onClickCallback;
      OnPressCallback onPressCallback;
      OnReleaseCallback onReleaseCallback;

    public:
      Component(const std::string& name, Component* parent);
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
      virtual void OnMousePressed(MousePressEvent& event, const Vec2& translatedPos);
      // Returns the hovered content
      virtual void OnMouseMoved(MouseMoveEvent& event, const Vec2& translatedPos);

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

      // Size of the sizeType, 1w will return 1 and not the size of the component
      const Vec2& GetSizeType() const;

      // Sets the sizeType size and not the actuall component size.
      Component& SetWidth(float width);
      Component& SetHeight(float height);
      Component& SetWidthSizeType(SizeType width);
      Component& SetHeightSizeType(SizeType height);

      // Faster to use since it sets all values and does a single remeasure.
      Component& SetSize(float width, float height, SizeType widthType, SizeType heightType, bool remeasure = true);

      const Style& GetNormalStyle() const; 
      const Style& GetHoverStyle() const; 
      const Style& GetPressStyle() const; 

      Component& SetNormalStyle(const Style& style); 
      Component& SetHoverStyle(const Style& style); 
      Component& SetPressStyle(const Style& style); 

      virtual bool IsMouseInside(const Vec2& parentMouse) const;
      virtual void SetPosition(const Vec2& pos);
      void SetSize(const Vec2& size);

      virtual void SetGUIScene(GUIScene* scene);

      // These four will only be called if the component is focusable
      virtual void OnFocused();
      virtual void OnUnfocused();
      virtual void Focused(){}
      virtual void Unfocused(){}
      virtual void MouseEntered(){}
      virtual void MouseExited(){}

      virtual void ChildChangedFocus(bool focused) {}

      virtual bool UsingMouse();

      // These functions will only be called if the component is focused
      virtual void MousePressed(MousePressEvent& event, const Vec2& translatedPos);
      virtual void MouseReleased(MouseReleaseEvent& event, const Vec2& translatedPos);
      virtual void MouseMoved(MouseMoveEvent& event, const Vec2& translatedPos){}
      virtual void KeyPressed(KeyPressEvent& event){}
      virtual void KeyReleased(KeyReleaseEvent& event){}
      virtual void KeyTyped(KeyTypeEvent& event){}

      Component* GetRootNode();
      Vec2 GetTotalPadding() const;
      const TLBR& GetMargin() const;
      const TLBR& GetPadding() const;
      const TLBR& GetBorder() const;
      Vec2 GetContentSize() const;
      const std::string& GetName() const;

      uint GetComponentDepth() const;

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
