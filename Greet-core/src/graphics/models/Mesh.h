#pragma once

#include <internal/GreetGL.h>
#include <math/Maths.h>
#include <logging/Logger.h>
#include <map>
#include <vector>
#include <graphics/textures/Texture.h>
#include <graphics/models/MeshData.h>
#include <utils/ErrorHandle.h>

namespace Greet {
	class Mesh
	{
	private:
		// Location, vbo
		std::map<uint,uint> m_vbos;
		uint m_vaoId;
		uint m_iboId;
		uint m_vertexCount;
		uint m_indexCount;
		uint m_drawMode;
		bool m_culling = true;
		bool m_clockwise = false;

	public:
		Mesh(const Vec3* vertices, uint vertexCount, const uint* indices, uint indexCount);
		Mesh(MeshData* data);
    Mesh(const Mesh& mesh);
		void init(const Vec3* vertices, uint vertexCount, const uint* indices, uint indexCount);
    Mesh& operator=(const Mesh& mesh);
		virtual ~Mesh();
		inline uint GetVAO()  const { return m_vaoId; };

		void Render() const;

		void Bind() const;
		void Unbind() const;

		inline void SetDrawMode(uint drawMode) { m_drawMode = drawMode; }
		inline void SetClockwiseRender(bool clockwise) { m_clockwise = clockwise; }
		inline bool IsClockwiseRender() const { return m_clockwise; }

		inline void SetEnableCulling(bool culling) { m_culling = culling; }
		inline bool IsEnableCulling() const { return m_culling; }
		void AddAttribute(uint location, const Vec3* data);
		void AddAttribute(uint location, const Vec2* data);
		//void addAttribute(uint location, uint attributeSize, const float* data);
		void AddAttribute(uint location, uint attributeSize, const uint* data);
		void AddAttribute(AttributeDataBase* data);
		void SetDefaultAttribute4f(uint location, const Vec4& data);
		void SetDefaultAttribute3f(uint location, const Vec3& data);
	private:
		void EnableAttributes() const;
		void DisableAttributes() const;
	};
}
