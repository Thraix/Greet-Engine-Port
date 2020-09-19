#pragma once

#include <math/Maths.h>
#include <graphics/models/Material.h>
#include <graphics/models/Mesh.h>
#include <event/ViewportEvent.h>
#include <math/Line.h>

namespace Greet
{
  struct Transform2DComponent
  {
    Mat3 transform;
  };

  struct Transform3DComponent
  {
    Mat4 transform;
    Transform3DComponent(const Mat4& transform)
      : transform{transform}
    {}

    Transform3DComponent(const Vec3f& pos, const Vec3f& scale, const Vec3f& rot)
      : transform{Mat4::TransformationMatrix(pos, rot, scale)}
    {}
  };

  struct Camera3DComponent {

    public:
      bool primary;
    private:
      float fov;
      float near;
      float far;
      Vec3f cameraPos;
      Mat4 projectionMatrix;
      Mat4 viewMatrix;

      Mat4 invPVMatrix;

    public:
      Camera3DComponent(const Mat4& viewMatrix, float fov, float near, float far, bool primary)
        :
        primary{primary}, fov{fov}, near{near}, far{far},
        projectionMatrix{Mat4::PerspectiveViewport(fov, near, far)},
        viewMatrix{viewMatrix},
        invPVMatrix{~(projectionMatrix * viewMatrix)}
      {}

      const Mat4& GetViewMatrix() const { return viewMatrix;}
      const Mat4& GetProjectionMatrix() const { return projectionMatrix; }

      void SetProjectionMatrix(const Mat4& amProjectionMatrix)
      {
        projectionMatrix = amProjectionMatrix;
        invPVMatrix = ~(projectionMatrix * viewMatrix);
      }

      void SetViewMatrix(const Mat4& amViewMatrix)
      {
        Mat4 invMatrix = ~amViewMatrix;
        cameraPos = Vec3f{invMatrix.columns[3]};
        viewMatrix = amViewMatrix;
        invPVMatrix = ~(projectionMatrix * viewMatrix);
      }

      void SetShaderUniforms(const Ref<Shader>& shader) const
      {
        shader->SetUniform3f("uCameraPos", cameraPos);
        shader->SetUniformMat4("uViewMatrix", viewMatrix);
        shader->SetUniformMat4("uProjectionMatrix", projectionMatrix);
        shader->SetUniformMat4("uInvPVMatrix", invPVMatrix);
      }

      void ViewportResize(ViewportResizeEvent& event)
      {
        SetProjectionMatrix(Mat4::Perspective(event.GetAspect(), fov, near, far));
      }

      Vec3f GetWorldToScreenCoordinate(const Vec3f& coordinate) const
      {
        return projectionMatrix * (viewMatrix * coordinate);
      }

      Line GetScreenToWorldCoordinate(const Vec2f& screenPos) const
      {
        Line line;
        line.pos = invPVMatrix * Vec3f(screenPos.x, screenPos.y, -1.0);
        Vec3f far = invPVMatrix * Vec3f(screenPos.x, screenPos.y, 1.0);
        line.dir = far - line.pos;
        return line;
      }
  };

  struct TagComponent
  {
    std::string tag;
  };

  struct MeshComponent
  {
    Ref<Mesh> mesh;
    MeshComponent(const Ref<Mesh>& mesh)
      : mesh{mesh}
    {}
  };

  struct MaterialComponent
  {
    Ref<Material> material;
    MaterialComponent(const Ref<Material>& material)
      : material{material}
    {}
  };
}

