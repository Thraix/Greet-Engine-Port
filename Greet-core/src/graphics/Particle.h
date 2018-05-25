#pragma once

#include <math/Vec2.h>
#include "Renderable2D.h"

namespace Greet{
	class Particle : public Renderable2D {
		friend class ParticlePool;
	private:
		Vec2 m_startPos;
		float m_minspeed;
		float m_maxspeed;
		float m_frameTime;
		Vec2 m_velocity;
		Vec2 m_acceleration;
		Vec2 m_pos;
		Vec2 m_size;
	private:
		Particle();
		bool Update(float timeElapsed);
		void Define(Vec2 pos, Vec2 size, Vec2 vel, Vec2 acc, uint color, float minspeed, float maxspeed, float frameTime, Sprite* sprite);

	public:
		inline Vec2 GetVelocity() const { return m_velocity; }
		inline Vec2 GetAcceleration() const { return m_acceleration; }
		inline float GetMaxSpeed() const { return m_maxspeed; }
		inline float GetMinSpeed() const { return m_minspeed; }
		inline double GetFrameTime() const { return m_frameTime; }
		virtual ~Particle();
	};

}
