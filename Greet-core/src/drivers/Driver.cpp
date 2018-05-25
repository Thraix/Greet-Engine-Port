#pragma once

#include "Driver.h"

#include <logging/Log.h>

namespace Greet {
	Driver::Driver(float time, DriverListener* listener) 
		: m_time(time), m_elapsed(0), m_listener(listener)
	{
	
	}

	Driver::~Driver() 
	{
		delete m_listener;
	}

	void Driver::Preupdate(float timeElapsed) 
	{ 
		m_elapsedSinceStart += timeElapsed;
		m_elapsed = timeElapsed;
		if (m_elapsedSinceStart >= m_time)
		{
			m_elapsed = m_time - m_elapsedSinceStart + timeElapsed;
			m_elapsedSinceStart = m_time;
		}
	}

	bool Driver::Postupdate()
	{
		if (m_elapsedSinceStart >= m_time)
		{
			OnFinished();
			return true;
		}
		return false;
	}

	void Driver::OnFinished() 
	{
		m_listener->OnFinished();
	}
}