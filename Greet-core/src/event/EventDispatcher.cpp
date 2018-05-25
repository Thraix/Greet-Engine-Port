#include "EventDispatcher.h"

namespace Greet {

	std::map<uint, std::vector<MouseListener*>, std::greater<uint>> EventDispatcher::m_mouseListeners;
	std::map<uint, std::vector<KeyListener*>, std::greater<uint>> EventDispatcher::m_keyListeners;

	void EventDispatcher::AddMouseListener(uint priority, MouseListener& listener)
	{
		for (auto it = m_mouseListeners.begin();it != m_mouseListeners.end(); it++)
		{
			if (it->first == priority)
			{
				it->second.push_back(&listener);
			}
		}
		std::vector<MouseListener*> listeners;
		listeners.push_back(&listener);
		m_mouseListeners.insert(std::pair<uint, std::vector<MouseListener*>>(priority,listeners));
	}

	void EventDispatcher::AddKeyListener(uint priority, KeyListener& listener)
	{
		for (auto it = m_keyListeners.begin();it != m_keyListeners.end(); it++)
		{
			if (it->first == priority)
			{
				it->second.push_back(&listener);
				return;
			}
		}
		std::vector<KeyListener*> listeners;
		listeners.push_back(&listener);
		m_keyListeners.insert(std::pair<uint, std::vector<KeyListener*>>(priority, listeners));
	}

	void EventDispatcher::RemoveMouseListener(MouseListener& listener)
	{
		for (auto it = m_mouseListeners.begin();it != m_mouseListeners.end(); it++)
		{
			std::vector<MouseListener*> listeners = it->second;
			for (auto it2 = listeners.begin(); it2 != listeners.end(); it2++)
			{
				if ((*it2)->m_id == listener.m_id)
				{
					listeners.erase(it2);
					return;
				}
			}
		}
		Log::Warning("Could not remove listener, doesn't exist.");
	}

	void EventDispatcher::RemoveKeyListener(KeyListener& listener)
	{
		for (auto it = m_keyListeners.begin();it != m_keyListeners.end(); it++)
		{
			std::vector<KeyListener*> listeners = it->second;
			for (auto it2 = listeners.begin(); it2 != listeners.end(); it2++)
			{
				if ((*it2)->m_id == listener.m_id)
				{
					listeners.erase(it2);
					return;
				}
			}
		}
		Log::Warning("Could not remove listener, doesn't exist.");
	}

	void EventDispatcher::OnMousePressed(const MousePressedEvent& e)
	{
		for (auto it = m_mouseListeners.begin();it != m_mouseListeners.end(); it++)
		{
			std::vector<MouseListener*> listeners = it->second;
			for (auto it2 = listeners.begin(); it2 != listeners.end(); it2++)
			{
				if ((*it2)->OnPressed(e))
					return;
			}
		}
	}

	void EventDispatcher::OnMouseReleased(const MouseReleasedEvent& e)
	{
		bool done = false;
		for (auto it = m_mouseListeners.begin();it != m_mouseListeners.end(); it++)
		{
			std::vector<MouseListener*> listeners = it->second;
			for (auto it2 = listeners.begin(); it2 != listeners.end(); it2++)
			{
				if ((*it2)->OnReleased(e))
					return;
			}
		}
	}

	void EventDispatcher::OnMouseMoved(const MouseMovedEvent& e)
	{
		for (auto it = m_mouseListeners.begin();it != m_mouseListeners.end(); it++)
		{
			std::vector<MouseListener*> listeners = it->second;
			for (auto it2 = listeners.begin(); it2 != listeners.end(); it2++)
			{
				if ((*it2)->OnMoved(e))
					return;
			}
		}
	}

	void EventDispatcher::OnMouseScrolled(const MouseScrollEvent& e)
	{
		for (auto it = m_mouseListeners.begin();it != m_mouseListeners.end(); it++)
		{
			std::vector<MouseListener*> listeners = it->second;
			for (auto it2 = listeners.begin(); it2 != listeners.end(); it2++)
			{
				if ((*it2)->OnScroll(e))
					return;
			}
		}
	}

	void EventDispatcher::OnKeyPressed(const KeyPressedEvent& e)
	{
		for (auto it = m_keyListeners.begin();it != m_keyListeners.end(); it++)
		{
			std::vector<KeyListener*> listeners = it->second;
			for (auto it2 = listeners.begin(); it2 != listeners.end(); it2++)
			{
				if ((*it2)->OnPressed(e))
				{
					return;
				}
			}
		}
	}

	void EventDispatcher::OnKeyReleased(const KeyReleasedEvent& e)
	{
		for (auto it = m_keyListeners.begin();it != m_keyListeners.end(); it++)
		{
			std::vector<KeyListener*> listeners = it->second;
			for (auto it2 = listeners.begin(); it2 != listeners.end(); it2++)
			{
				if ((*it2)->OnReleased(e))
					return;
			}
		}
	}

	void EventDispatcher::OnKeyTyped(const KeyTypedEvent& e)
	{
		for (auto it = m_keyListeners.begin();it != m_keyListeners.end(); it++)
		{
			std::vector<KeyListener*> listeners = it->second;
			for (auto it2 = listeners.begin(); it2 != listeners.end(); it2++)
			{
				if ((*it2)->OnTyped(e))
					return;
			}
		}
	}
}