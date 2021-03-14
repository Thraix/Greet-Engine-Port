#pragma once

#include <map>
#include <vector>
#include <memory>
#include <graphics/buffers/Buffer.h>
#include <graphics/buffers/VertexArray.h>
#include <graphics/models/MeshData.h>
#include <math/Maths.h>
#include <utils/BoundingBox.h>

namespace Greet {


  class Mesh
  {
    private:
      // Location, vbo
      std::map<uint, Ref<Buffer>> m_vbos;

      Ref<VertexArray> vao;
      Ref<Buffer> ibo;

      uint m_vertexCount;
      uint m_indexCount;
      DrawType m_drawMode;
      bool m_culling = true;
      bool m_clockwise = false;
      bool wireframe = false;

      BoundingBox boundingBox;

    public:
      Mesh(const Pointer<Vec3f>& vertices, const Pointer<uint>& indices);
      Mesh(const MeshData& data);

      void Render() const;

      void Bind() const;
      void Unbind() const;

      inline void SetDrawMode(DrawType drawType) { m_drawMode = drawType; }
      inline void SetClockwiseRender(bool clockwise) { m_clockwise = clockwise; }
      inline bool IsClockwiseRender() const { return m_clockwise; }

      inline void SetEnableCulling(bool culling) { m_culling = culling; }
      inline bool IsEnableCulling() const { return m_culling; }

      inline void SetEnableWireframe(bool _wireframe) { wireframe = _wireframe; }
      inline bool IsEnableWireframe() const { return wireframe; }

      void AddAttribute(const BufferAttribute& attribute, const Pointer<char>& data);

      void SetDefaultAttribute4f(uint location, const Vec4f& data);
      void SetDefaultAttribute3f(uint location, const Vec3f& data);

      const BoundingBox& GetBoundingBox() const { return boundingBox; }
    private:
      bool HasVBO(uint location) const;
  };
}
