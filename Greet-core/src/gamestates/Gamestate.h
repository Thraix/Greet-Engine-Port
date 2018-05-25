#if 0
#pragma once

#include <graphics/layers/Layer.h>

namespace Greet {
	class GameState : public Layer
	{
	friend class GameStateManager;
	protected:
		GameState* parent;
	public:

	protected:
		virtual void Init() = 0;
		virtual void SetUniforms(){}
		virtual void RenderBefore(){};
		virtual void RenderAfter(){};
		virtual void Update(float timeElapsed) = 0;

	public:
		GameState(Renderer2D* renderer, Shader* shader, Mat3 projectionMatrix);
		virtual ~GameState(){ }
	};
}
#endif
