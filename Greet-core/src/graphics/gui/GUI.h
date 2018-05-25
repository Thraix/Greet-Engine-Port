#pragma once

#include <graphics/layers/Group.h>
#include <math/Maths.h>
#include "LTRB.h"
#include <event/KeyEvent.h>
#include <event/MouseEvent.h>
#include <utils/ColorUtils.h>
#include <listeners/OnClickListener.h>
#include <algorithm>
#include <graphics/textures/TextureManager.h>
#include <drivers/DriverAdapter.h>
#include <graphics/renderers/GUIRenderer.h>

#define MOUSE_INSIDE(mouse,xPos,yPos,width,height) (mouse).x >= (xPos) && (mouse).x < (xPos) + (width) && (mouse).y >= (yPos) && (mouse).y < (yPos) + (height)
#define MOUSE_INSIDE_GUI(mouse,width,height) (mouse).x >= 0 && (mouse).x < (width) && (mouse).y >= 0 && (mouse).y < (height)

#define GUI_DEFAULT_BACKGROUND Vec4(0.0f,0.0f, 0.25f,1.0f)

namespace Greet{
	class GUI
	{
		friend class GUILayer;
		protected:

			Mat3 m_transformationMatrix;
			std::vector<GUI*> m_children;
			GUI* m_parent;
			// Mouse is inside GUI
			bool m_mouseInside = false;
			// Can be focused
			bool m_enabled = true;
			// is the GUI focused
			bool m_focused = false;
			//Renderable2D* m_background;
		public:
			LTRB m_margin;
			Vec2 m_position;
			Vec2 m_size;
			Vec4 m_backgroundColor;
			bool m_renderBackground = true;
			std::vector<OnClickListener*> m_onClickListeners;
			static Sprite* m_mask;
		private:
			void SetFocused(bool focused);

		public:
			GUI(const Vec2& position, const Vec2& size);
			GUI(const Vec2& position, const Vec2& size, const LTRB& margin);
			virtual ~GUI();

			void Add(GUI* renderable);
			void Remove(GUI* renderable);
			bool IsFocused() const { return m_focused; }
			void SetBackgroundColor(const Vec4& bgColor) { m_backgroundColor = bgColor;}
			virtual bool Update(float timeElapsed);
			virtual void RenderBackground(GUIRenderer* renderer) const;
			virtual void Begin(GUIRenderer* renderer) const;
			virtual void Submit(GUIRenderer* renderer) const = 0;
			virtual void End(GUIRenderer* renderer) const;
			virtual void OnFocused(bool focused) {};
			virtual void OnMouseEnter() { };
			virtual void OnMouseExit() { };
			virtual bool OnPressed(const KeyPressedEvent& event);
			virtual bool OnReleased(const KeyReleasedEvent& event);
			virtual bool OnTyped(const KeyTypedEvent& event);
			// Returns the pressed GUI
			virtual GUI* OnPressed(const MousePressedEvent& event, Vec2 relativeMousePos);
			// Returns the released GUI
			virtual GUI* OnReleased(const MouseReleasedEvent& event, Vec2 relativeMousePos);

			virtual bool OnMoved(const MouseMovedEvent& event, Vec2 relativeMousePos);

			// Pack the GUI to fit the children.
			virtual void Pack();
			virtual Vec2 GetMaxChildrenPos(bool packing) const;

			void AddOnClickListener(OnClickListener* onClick) { m_onClickListeners.push_back(onClick); };
			void RemoveOnClickListener(OnClickListener* onClick) { m_onClickListeners.erase(std::remove(m_onClickListeners.begin(), m_onClickListeners.end(),onClick), m_onClickListeners.end()); };

			virtual bool IsInside(const Vec2& position) const;
			Vec2 TranslateMouse(const Vec2& mousePos, GUI* target) const;
			const Vec2& GetRealPosition() const;
			inline const Vec2& GetPosition() const { return m_position;}
			inline const Vec2& GetSize() const { return m_size;}
			inline const Vec4& GetBackgroundColor() const {return m_backgroundColor;}
			inline bool IsRenderBackground() const { return m_renderBackground;}

			static bool IsInside(const Vec2& pos, const Vec2& size, const Vec2 mouse);
	};
}
