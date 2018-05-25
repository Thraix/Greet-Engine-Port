#include "GUI.h"

namespace Greet {
	
	Sprite* GUI::m_mask;

	GUI::GUI(const Vec2& position, const Vec2& size)
	: m_position(position), m_size(size), m_margin(LTRB()), m_backgroundColor(GUI_DEFAULT_BACKGROUND)
	{
		m_transformationMatrix = Mat3::Translate(m_position);
		if (m_mask == NULL)
			m_mask = new Sprite(TextureManager::Get2D("guimask"));
	}
	
	GUI::GUI(const Vec2& position, const Vec2& size, const LTRB& margin)
		: m_position(position), m_size(size), m_margin(margin), m_backgroundColor(GUI_DEFAULT_BACKGROUND)
	{
		m_transformationMatrix = Mat3::Translate(m_position);
		if (m_mask == NULL)
			m_mask = new Sprite(TextureManager::Get2D("guimask"));
	}

	GUI::~GUI()
	{
	}

	void GUI::Add(GUI* renderable)
	{
		if (renderable->m_parent != nullptr)
		{
			Log::Warning("Tried adding GUI that already has a parent.");
		}
		m_children.push_back(renderable);
		renderable->m_parent = this;
	}

	void GUI::Remove(GUI* renderable)
	{
		for (auto it = m_children.begin();it != m_children.end();it++)
		{
			if (*it == renderable)
			{
				(*it)->m_parent = nullptr;
				m_children.erase(it);
				return;
			}
		}
	}

	bool GUI::OnPressed(const KeyPressedEvent& e)
	{ 
		return false;
	}

	bool GUI::OnReleased(const KeyReleasedEvent& e)
	{ 
		return false;
	}

	bool GUI::OnTyped(const KeyTypedEvent& e)
	{
		return false;
	}

	GUI* GUI::OnPressed(const MousePressedEvent& event, Vec2 relativeMousePos) 
	{ 
		// If we are not inside the GUI return
		if (!m_enabled || !m_mouseInside)
			return NULL; // No GUI that can be focused

		for (uint i = 0;i < m_children.size();i++)
		{
			GUI* gui = m_children[i];
			// Check if mouse is within child-GUIs
			if (gui->OnPressed(event, TranslateMouse(relativeMousePos, gui)))
				return gui;
		}
		for (auto it = m_onClickListeners.begin(); it != m_onClickListeners.end();++it)
		{
			(*it)->OnClick(this);
		}
		return this;
	}

	GUI* GUI::OnReleased(const MouseReleasedEvent& event, Vec2 relativeMousePos) 
	{ 
		if (!m_enabled)
			return NULL;
		for (uint i = 0;i < m_children.size();i++)
		{
			GUI* gui = m_children[i];
			// Check if mouse is within child-GUIs
			gui->OnReleased(event, TranslateMouse(relativeMousePos, gui));
		}
		return NULL;
	}

	bool GUI::OnMoved(const MouseMovedEvent& event, Vec2 relativeMousePos)
	{
		if (IsInside(relativeMousePos))
		{
			if (!m_mouseInside)
				OnMouseEnter();
			m_mouseInside = true;
		}
		else
		{
			if (m_mouseInside)
				OnMouseExit();
			m_mouseInside = false;
		}
		if (m_mouseInside)
		{
			for (uint i = 0;i < m_children.size();i++)
			{
				GUI* gui = m_children[i];
				gui->OnMoved(event, TranslateMouse(relativeMousePos, gui));
			}
		}
		return m_mouseInside;
	}


	bool GUI::Update(float timeElapsed) 
	{
		if(m_parent == nullptr)
			m_transformationMatrix = Mat3::Translate(m_position);
		else
			m_transformationMatrix = Mat3::Translate(m_position + Vec2(m_parent->m_margin.left, m_parent->m_margin.top));
		

		// Update children
		bool update = false;
		for (uint i = 0; i < m_children.size(); i++)
			update |= m_children[i]->Update(timeElapsed);
		return update;
	}

	void GUI::RenderBackground(GUIRenderer* renderer) const
	{
		renderer->SubmitRect(Vec2(0, 0), m_size, m_backgroundColor);
	}

	void GUI::Begin(GUIRenderer* renderer) const
	{
		renderer->PushMatrix(m_transformationMatrix);
		renderer->PushViewport(Vec2(0,0), m_size);
		if (m_renderBackground)
			RenderBackground(renderer);
	}

	void GUI::End(GUIRenderer* renderer) const
	{
		for (uint i = 0; i < m_children.size(); i++)
		{
			m_children[i]->Begin(renderer);
			m_children[i]->Submit(renderer);
			m_children[i]->End(renderer);
		}
		renderer->PopViewport();
		renderer->PopMatrix();
	}

	void GUI::Pack()
	{
		m_size = GetMaxChildrenPos(true)+Vec2(m_margin.left+m_margin.right, m_margin.top+m_margin.bottom);
	}

	Vec2 GUI::GetMaxChildrenPos(bool packing) const
	{
		Vec2 pos = m_position + m_size;
		if (packing)
			pos = Vec2(0, 0);
		for (uint i = 0; i < m_children.size();++i)
		{
			Vec2 childPos = m_children[i]->m_position + m_children[i]->m_size;
			pos.x = pos.x < childPos.x ? childPos.x : pos.x;
			pos.y = pos.y < childPos.y ? childPos.y : pos.y;
		}
		return pos;
	}

	const Vec2& GUI::GetRealPosition() const
	{
		if(m_parent)
			return m_parent->GetPosition()+m_position;
		return m_position;
	}

	void GUI::SetFocused(bool focused)
	{
		if (m_focused != focused)
		{
			OnFocused(focused);
			m_focused = focused;
		}
	}

	bool GUI::IsInside(const Vec2& position) const
	{
		return IsInside(Vec2(0, 0), m_size, position);
	}

	Vec2 GUI::TranslateMouse(const Vec2& mousePos, GUI* target) const
	{	
		return mousePos - target->m_position - Vec2(m_margin.left, m_margin.top);
	}

	bool GUI::IsInside(const Vec2& pos1, const Vec2& pos2, const Vec2 mouse)
	{
		return mouse.x >= pos1.x && mouse.x < pos2.x && mouse.y >= pos1.y && mouse.y < pos2.y;
	}
}
