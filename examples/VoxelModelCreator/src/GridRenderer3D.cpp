#include "GridRenderer3D.h"

namespace vmc
{
  using namespace Greet;

  GridRenderer3D::GridRenderer3D()
    : Renderer3D(), lineShader(Shader::FromFile("res/shaders/simple.shader")), vao(), vbo(2 * sizeof(Vec3<float>), BufferType::ARRAY, BufferDrawType::DYNAMIC), ibo(2 * sizeof(uint), BufferType::INDEX, BufferDrawType::STATIC)
  {
    MeshData meshdata = MeshFactory::Cube2(0.5f, 0.5f, 0.5f, 1, 1, 1);
    std::vector<Vec2> texCoords(6 * 4);
    for (int i = 0;i < 6;i++)
    {
      texCoords[i * 4 + 0] = Vec2(0, 0);
      texCoords[i * 4 + 1] = Vec2(1, 0);
      texCoords[i * 4 + 2] = Vec2(1, 1);
      texCoords[i * 4 + 3] = Vec2(0, 1);
    }

    meshdata.AddAttribute(AttributeData(ATTRIBUTE_TEXCOORD, texCoords));
    mesh = new Mesh(meshdata);
    material = new Material(Shader::FromFile("res/shaders/voxel.shader"));
    //material->AddUniform<uint>(Uniform1ui("color"));
    emodel = new EntityModel(mesh, material, 0, 0, 0, 1, 1, 1, 0, 0, 0);


    // For drawing lines...
    vao.Enable();
    vbo.Enable();
    vbo.UpdateData(nullptr);

    GLCall(glEnableVertexAttribArray(0));

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3<float>), 0));
    vbo.Disable();
    vao.Disable();

    m_indices = new uint[2]{ 0,1 };
    ibo.Enable();
    ibo.UpdateData(m_indices);
    ibo.Disable();

  }

  void GridRenderer3D::Begin(Camera* camera)
  {
    Renderer3D::Begin(camera);
    glLineWidth(1.0f);
  }

  void GridRenderer3D::DrawCube(Camera* camera, const Vec3<float>& pos, const Vec3<float>& size, uint color, bool culling)
  {
    material->SetColor(ColorUtils::ColorHexToVec4(color));
    mesh->SetEnableCulling(false);
    emodel->SetScale(size);
    emodel->SetPosition(pos);
    emodel->SetRotation(Vec3<float>(0, 0, 0));
    emodel->UpdateTransformation();
    emodel->BindShader(this, camera);
    emodel->PreRender();
    emodel->Render(this, camera);
    emodel->PostRender();
    emodel->UnbindShader(this, camera);
  }

  void GridRenderer3D::Submit(Camera* camera, const Cube& cube)
  {
    material->SetColor(ColorUtils::ColorHexToVec4(cube.color));
    mesh->SetEnableCulling(false);
    emodel->SetScale(Vec3<float>(1, 1, 1));
    emodel->SetPosition(cube.GetPosition());
    emodel->SetRotation(Vec3<float>(0, 0, 0));
    emodel->UpdateTransformation();
    emodel->GetMaterial()->Bind(camera);
    BindMatrices(emodel->GetMaterial()->GetShader(), camera);
    emodel->GetMesh()->Bind();

    emodel->GetMaterial()->GetShader()->SetUniformMat4("transformationMatrix", emodel->GetTransformationMatrix());
    emodel->GetMesh()->Render();
    emodel->GetMesh()->Unbind();
    emodel->GetMaterial()->GetShader()->Disable();
    //emodel->PreRender(this, camera);
    //emodel->Render(this, camera);
    //emodel->PostRender(this, camera);
  }

  void GridRenderer3D::DrawLineCube(Camera* camera, const Vec3<float>& pos, const Vec3<float>& size, const Vec4& color)
  {
    DrawLine(camera, Vec3(pos.x, pos.y, pos.z), Vec3(pos.x + size.x, pos.y, pos.z), color);
    DrawLine(camera, Vec3(pos.x, pos.y + size.y, pos.z), Vec3(pos.x + size.x, pos.y + size.y, pos.z), color);
    DrawLine(camera, Vec3(pos.x, pos.y + size.y, pos.z + size.z), Vec3(pos.x + size.x, pos.y + size.y, pos.z + size.z), color);
    DrawLine(camera, Vec3(pos.x, pos.y, pos.z + size.z), Vec3(pos.x + size.x, pos.y, pos.z + size.z), color);

    DrawLine(camera, Vec3(pos.x, pos.y, pos.z), Vec3(pos.x, pos.y+size.y, pos.z), color);
    DrawLine(camera, Vec3(pos.x + size.x, pos.y, pos.z), Vec3(pos.x + size.x, pos.y + size.y, pos.z), color);
    DrawLine(camera, Vec3(pos.x + size.x, pos.y, pos.z + size.z), Vec3(pos.x + size.x, pos.y + size.y, pos.z + size.z), color);
    DrawLine(camera, Vec3(pos.x, pos.y, pos.z + size.z), Vec3(pos.x, pos.y + size.y, pos.z + size.z), color);

    DrawLine(camera, Vec3(pos.x, pos.y, pos.z), Vec3(pos.x, pos.y, pos.z+size.z), color);
    DrawLine(camera, Vec3(pos.x, pos.y + size.y, pos.z), Vec3(pos.x, pos.y + size.y, pos.z+size.z), color);
    DrawLine(camera, Vec3(pos.x + size.x, pos.y + size.y, pos.z), Vec3(pos.x + size.x, pos.y + size.y, pos.z + size.z), color);
    DrawLine(camera, Vec3(pos.x + size.x, pos.y, pos.z), Vec3(pos.x + size.x, pos.y, pos.z + size.z), color);
  }

  void GridRenderer3D::DrawLine(Camera* camera, const Vec3<float>& start, const Vec3<float>& end, const Vec4& color)
  {
    lineShader->Enable();
    lineShader->SetUniformMat4("projectionMatrix", camera->GetProjectionMatrix());
    lineShader->SetUniformMat4("viewMatrix", camera->GetViewMatrix());
    lineShader->SetUniform4f("mat_color", color);

    vbo.Enable();
    Vec3<float>* buffer = (Vec3<float>*)vbo.MapBuffer();

    buffer[0] = start;
    buffer[1] = end;

    vbo.UnmapBuffer();
    vbo.Disable();

    vao.Enable();
    ibo.Enable();
    GLCall(glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, NULL));
    ibo.Disable();
    vao.Disable();

    lineShader->Disable();
  }
}
