#if 0
#pragma once

#include <vector>
#include <gamestates/Gamestate.h>
#include <logging/Log.h>
#include <internal/GreetTypes.h>

namespace Greet {
	class GameStateManager
	{
	private:
		static GameState* m_gamestate;
	public:
		static void Set(GameState* gamestate);
		static void Render();
		static void Update(float timeElapsed);
		static GameState* Get() { return m_gamestate; }
		static void GoBack();
		static void Destroy();
	private:
		GameStateManager(){}
	};
}
#endif
