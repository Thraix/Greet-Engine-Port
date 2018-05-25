#if 0
#include "Layer.h"

namespace greet {

	template<class T>
	Layer<T>::Layer(Renderer2D* renderer, Shader* shader, Mat3 projectionMatrix)
		: m_renderer(renderer), m_shader(shader), m_projectionMatrix(projectionMatrix)
	{
		GLint texIDs[32];
		for (int i = 0; i < 32; i++)
		{
			texIDs[i] = i;
		}
		m_shader->enable();
		m_shader->setUniformMat3("pr_matrix", m_projectionMatrix);
		m_shader->setUniform1iv("textures", 32, texIDs);
		m_shader->disable();
	}

	template<class T>
	Layer<T>::~Layer()
	{
		delete m_shader;
		delete m_renderer;

		for (uint i = 0; i < m_renderables.size(); i++)
			delete m_renderables[i];
	}

	template<class T>
	void Layer<T>::add(T* renderable)
	{
		m_renderables.push_back(renderable);
	}

	template<class T>
	void Layer<T>::render() const
	{
		m_shader->enable();
		setUniforms();
		m_renderer->begin();
		uint size = m_renderables.size();
		for (uint i = 0; i < size; i++)
		{
			m_renderables[i]->begin(m_renderer);
			m_renderables[i]->submit(m_renderer);
			m_renderables[i]->end(m_renderer);
		}
		m_renderer->end();
		m_renderer->flush();
		m_shader->disable();
	}

	template<class T>
	void Layer<T>::update(float timeElapsed)
	{
		uint size = m_renderables.size();
		for (uint i = 0; i < size; i++)
			m_renderables[i]->update(timeElapsed);
	}

	template<class T>
	void Layer<T>::setProjectionMatrix(Mat3 projectionMatrix)
	{
		 m_projectionMatrix = projectionMatrix; 
		 m_shader->enable();
		 m_shader->setUniformMat3("pr_matrix", m_projectionMatrix);
		 m_shader->disable();
	}
}
#endif
