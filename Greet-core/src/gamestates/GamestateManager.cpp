#if 0
#include "GamestateManager.h"

namespace Greet{

	GameState* GameStateManager::m_gamestate;

	void GameStateManager::Set(GameState* gamestate)
	{
		if (m_gamestate!=nullptr)
			gamestate->parent = m_gamestate;
		m_gamestate = gamestate;
		m_gamestate->Init();
	}

	void GameStateManager::Render()
	{
		m_gamestate->RenderBefore();
		m_gamestate->Render();
		m_gamestate->RenderAfter();
	}

	void GameStateManager::Update(float timeElapsed)
	{
		m_gamestate->Update(timeElapsed);
	}

	void GameStateManager::GoBack()
	{
		if (m_gamestate->parent != nullptr)
		{
			GameState* state = m_gamestate->parent;
			delete m_gamestate;
			m_gamestate = state;
		}
	}

	void GameStateManager::Destroy()
	{
		delete m_gamestate;
	}

}
#endif
