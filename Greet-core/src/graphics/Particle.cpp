#include "Particle.h"

namespace Greet {

	Particle::Particle()
	{
	}

	Particle::~Particle()
	{
		delete m_sprite;
	}

	void Particle::Define(Vec2 pos, Vec2 size, Vec2 vel, Vec2 acc, uint color, float minspeed, float maxspeed, float frameTime, Sprite* sprite)
	{
		m_startPos = pos;
		m_pos = pos;
		m_size = size;
		m_color = color;
		m_velocity = vel;
		m_acceleration = acc;
		m_minspeed = fabsf(minspeed);
		m_maxspeed = fabsf(maxspeed);
		m_frameTime = frameTime;
		m_sprite = sprite;
	}

	bool Particle::Update(float elapsedTime)
	{
		m_frameTime -= elapsedTime;

		m_velocity *= m_acceleration;
		if (m_velocity.Length() > m_maxspeed)
		{
			m_velocity.Normalize() *= m_maxspeed;
		}
		else if (m_velocity.Length() < m_minspeed)
		{
			m_velocity.Normalize() *= m_minspeed;
		}
		m_pos += m_velocity;
		return m_frameTime<=0;
	}
}
