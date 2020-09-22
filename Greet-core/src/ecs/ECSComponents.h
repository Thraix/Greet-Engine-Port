#pragma once

#include <ecs/NativeScript.h>
#include <event/ViewportEvent.h>
#include <graphics/Sprite.h>
#include <graphics/models/Material.h>
#include <graphics/models/Mesh.h>
#include <graphics/models/MeshFactory.h>
#include <graphics/RenderCommand.h>
#include <math/Line.h>
#include <math/Maths.h>

namespace Greet
{
  struct Transform2DComponent
  {
    Mat3 transform;
    Transform2DComponent(const Mat3& transform)
      : transform{transform}
    {}

    Transform2DComponent(const Vec2f& pos, const Vec2f& scale, float rot)
      : transform{Mat3::TransformationMatrix(pos, scale, rot)}
    {}
  };

  struct Camera2DComponent {
    public:
      bool primary;
    private:
      Mat3 projectionMatrix;
      Mat3 viewMatrix;
      Vec2f cameraPos;
    public:

      Camera2DComponent(const Mat3& viewMatrix, bool primary)
        : primary{primary}, projectionMatrix{Mat3::OrthographicViewport()}, viewMatrix{viewMatrix}
      {}

      const Mat3& GetViewMatrix() const { return viewMatrix; }
      const Mat3& GetProjectionMatrix() const { return projectionMatrix; }

      void SetProjectionMatrix(const Mat3& amProjectionMatrix)
      {
        projectionMatrix = amProjectionMatrix;
      }

      void SetViewMatrix(const Mat3& amViewMatrix)
      {
        Mat3 invMatrix = ~viewMatrix;
        cameraPos = Vec2f{invMatrix.columns[2]};
        viewMatrix = amViewMatrix;
      }

      void SetShaderUniforms(const Ref<Shader>& shader) const
      {
        shader->SetUniform2f("uCameraPos", cameraPos);
        shader->SetUniformMat3("uViewMatrix", viewMatrix);
        shader->SetUniformMat3("uProjectionMatrix", projectionMatrix);
      }

      void ViewportResize(ViewportResizeEvent& event)
      {
        SetProjectionMatrix(Mat3::OrthographicViewport());
      }
  };

  struct Environment2DComponent
  {
    Ref<Shader> shader;
    Environment2DComponent(const Ref<Shader>& shader)
      : shader{shader}
    {}
  };

  struct SpriteComponent
  {
    Ref<Texture2D> texture;
    Vec2f texPos;
    Vec2f texSize;
    SpriteComponent(const Ref<Texture2D>& texture, const Vec2f& texPos, const Vec2f& texSize)
      : texture{texture}, texPos{texPos}, texSize{texSize}
    {}
  };

  struct Transform3DComponent
  {
    Mat4 transform;
    Transform3DComponent(const Mat4& transform)
      : transform{transform}
    {}

    Transform3DComponent(const Vec3f& pos, const Vec3f& scale, const Vec3f& rot)
      : transform{Mat4::TransformationMatrix(pos, scale, rot)}
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

  struct Environment3DComponent
  {
    private:
      Ref<Mesh> skyboxMesh;
    public:
      Ref<Shader> skybox;
      Ref<CubeMap> skyboxTexture;
      Vec3f lightPos = Vec3f(30.0, 20.0, 40.0);
      Color lightColor = Color{1.0f, 0.96f, 0.9f};
      float fogNearDistance = 100;
      float fogFarDistance = 140;
      Color fogColor = Color{0.125f, 0.125f, 0.125f};

      Environment3DComponent(const Ref<Shader>& skybox, const Ref<CubeMap>& skyboxTexture, const Vec3f& lightPos, const Color& lightColor, float fogNearDistance, float fogFarDistance, const Color& fogColor)
        : skyboxMesh{new Mesh{MeshFactory::Cube()}}, skybox{skybox}, skyboxTexture{skyboxTexture}, lightPos{lightPos}, lightColor{lightColor}, fogNearDistance{fogNearDistance}, fogFarDistance{fogFarDistance}, fogColor{fogColor}
      {
        skyboxMesh->SetClockwiseRender(true);
      }

      Environment3DComponent(const Ref<Shader>& skybox, const Ref<CubeMap>& skyboxTexture)
        : skyboxMesh{new Mesh{MeshFactory::Cube()}}, skybox{skybox}, skyboxTexture{skyboxTexture}
      {
        skyboxMesh->SetClockwiseRender(true);
      }

      Environment3DComponent(const Ref<Shader>& skybox)
        : skyboxMesh{new Mesh{MeshFactory::Cube()}}, skybox{skybox}
      {
        skyboxMesh->SetClockwiseRender(true);
      }

      Environment3DComponent(const Ref<CubeMap>& skyboxTexture)
        : skyboxMesh{new Mesh{MeshFactory::Cube()}}, skybox{ShaderFactory::ShaderSkybox()}, skyboxTexture{skyboxTexture}
      {
        skyboxMesh->SetClockwiseRender(true);
      }

      Environment3DComponent()
        : skyboxMesh{new Mesh{MeshFactory::Cube()}}, skybox{ShaderFactory::ShaderSkybox()}
      {
        skyboxMesh->SetClockwiseRender(true);
      }

      void Skybox(Camera3DComponent& camera)
      {
        RenderCommand::EnableDepthTest(false);
        skybox->Enable();
        camera.SetShaderUniforms(skybox);
        if(skyboxTexture)
          skyboxTexture->Enable(0);
        skyboxMesh->Bind();
        skyboxMesh->Render();
        skyboxMesh->Unbind();
        if(skyboxTexture)
          skyboxTexture->Disable();
        skybox->Disable();
        RenderCommand::ResetDepthTest();
      }

      void SetShaderUniforms(const Ref<Shader>& shader)
      {
        shader->SetUniform3f("uLightPosition", lightPos);
        shader->SetUniformColor3("uLightColor", lightColor);
        shader->SetUniform1f("uFogNearDistance", fogNearDistance);
        shader->SetUniform1f("uFogFarDistance", fogFarDistance);
        shader->SetUniformColor3("uFogColor", fogColor);
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

  struct NativeScriptComponent
  {
    Ref<NativeScript> script;
    bool created = false;

    NativeScriptComponent(const Ref<NativeScript>& script)
      : script{script}
    {}

    void Create() { script->Create(); created = true; }
    void Update(float timeElapsed) { if(created) script->Update(timeElapsed); }
    void OnEvent(Event& event) { if(created) script->OnEvent(event); }
    void Destroy() { script->Destroy(); created = false; }
  };
}

