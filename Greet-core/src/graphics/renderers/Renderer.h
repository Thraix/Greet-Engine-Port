#pragma once

#include <stack>
#include <internal/GreetGL.h>
#include <math/Maths.h>
#include <logging/Log.h>

namespace Greet{

	class Renderer
	{
	protected:
		std::stack<Mat3> m_transformationStack;
		const Mat3* m_transformationBack;

		Renderer()
		{
			m_transformationStack.push(Mat3::Identity());
			m_transformationBack = &m_transformationStack.top();
		}

	public:
		void PushMatrix(const Mat3 &matrix, bool override = false)
		{
			if (override)
				m_transformationStack.push(matrix);
			else
				m_transformationStack.push(*m_transformationBack * matrix);
			m_transformationBack = &m_transformationStack.top();

		}
		void PopMatrix()
		{
			if (m_transformationStack.size() > 1)
				m_transformationStack.pop();
			else
				Log::Warning("Trying to pop the last matrix.");
			m_transformationBack = &m_transformationStack.top();
		}

		const Mat3& GetMatrix() const
		{
			return *m_transformationBack;
		}
	};
}