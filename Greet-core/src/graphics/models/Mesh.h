#pragma once

#include <math/Maths.h>
#include <map>
#include <vector>
#include <graphics/models/MeshData.h>
#include <memory>
#include <graphics/buffers/VertexArray.h>
#include <graphics/buffers/Buffer.h>

namespace Greet {


  class Mesh
  {
    private:
      // Location, vbo
      std::map<uint, Buffer> m_vbos;

      VertexArray vao;
      Buffer ibo;

      uint m_vertexCount;
      uint m_indexCount;
      uint m_drawMode;
      bool m_culling = true;
      bool m_clockwise = false;
      bool wireframe = false;

    public:
      Mesh(const std::vector<Vec3<float>>& vertices, const std::vector<uint>& indices);
      Mesh(const MeshData& data);
      Mesh(Mesh&&) = default;
      Mesh& operator=(Mesh&&) = default;

      void Render() const;

      void Bind() const;
      void Unbind() const;

      inline void SetDrawMode(uint drawMode) { m_drawMode = drawMode; }
      inline void SetClockwiseRender(bool clockwise) { m_clockwise = clockwise; }
      inline bool IsClockwiseRender() const { return m_clockwise; }

      inline void SetEnableCulling(bool culling) { m_culling = culling; }
      inline bool IsEnableCulling() const { return m_culling; }

      inline void SetEnableWireframe(bool _wireframe) { wireframe = _wireframe; }
      inline bool IsEnableWireframe() const { return wireframe; }

      void AddAttribute(uint location, const Vec3<float>* data);
      void AddAttribute(uint location, const Vec2* data);
      //void addAttribute(uint location, uint attributeSize, const float* data);
      void AddAttribute(uint location, uint attributeSize, const uint* data);
      void AddAttribute(const AttributeData& attr);
      void AddAttribute(uint location, const void* data, uint dataSize, uint typeCount, uint glType, bool normalized);

      void SetDefaultAttribute4f(uint location, const Vec4& data);
      void SetDefaultAttribute3f(uint location, const Vec3<float>& data);
    private:
      bool HasVBO(uint location) const;
      void EnableAttributes() const;
      void DisableAttributes() const;
  };
}
