#include "GridRenderer3D.h"

namespace vmc
{
  using namespace Greet;

  GridRenderer3D::GridRenderer3D()
    : Renderer3D(), lineShader(Shader::FromFile("res/shaders/simple.shader"))
  {
    MeshData meshdata = MeshFactory::Cube({0.5f}, {1.0f});
    Pointer<Vec2f> texCoords(6 * 4);
    for (int i = 0;i < 6;i++)
    {
      texCoords[i * 4 + 0] = Vec2f(0, 0);
      texCoords[i * 4 + 1] = Vec2f(1, 0);
      texCoords[i * 4 + 2] = Vec2f(1, 1);
      texCoords[i * 4 + 3] = Vec2f(0, 1);
    }

    meshdata.AddAttribute({MESH_TEXCOORDS_LOCATION, BufferAttributeType::VEC2}, texCoords);
    mesh = new Mesh(meshdata);
    material = new Material(Shader::FromFile("res/shaders/voxel.shader"));
    //material->AddUniform<uint>(Uniform1ui("color"));
    emodel = new EntityModel(mesh, material, 0, 0, 0, 1, 1, 1, 0, 0, 0);


    // For drawing lines...
    vao = VertexArray::Create();
    vbo = VertexBuffer::CreateDynamic(nullptr, 2 * sizeof(Vec3f));
    vbo->SetStructure({{{0, BufferAttributeType::VEC3}}});
    vao->AddVertexBuffer(vbo);
    vbo->Disable();

    m_indices = new uint[2]{ 0, 1 };
    ibo = Buffer::Create(2 * sizeof(uint), BufferType::INDEX, BufferDrawType::STATIC);
    ibo->UpdateData(m_indices);
    ibo->Disable();

    vao->SetIndexBuffer(ibo);
    vao->Disable();
  }

  void GridRenderer3D::Begin(const Ref<Camera3D>& camera)
  {
    Renderer3D::Begin(camera);
    glLineWidth(1.0f);
  }

  void GridRenderer3D::DrawCube(const Ref<Camera3D>& camera, const Vec3f& pos, const Vec3f& size, uint color, bool culling)
  {
    material->SetColor(Color{color});
    mesh->SetEnableCulling(false);
    emodel->SetScale(size);
    emodel->SetPosition(pos);
    emodel->SetRotation(Vec3f(0, 0, 0));
    emodel->UpdateTransformation();
    emodel->BindShader();
    camera->SetShaderUniforms(emodel->GetMaterial()->GetShader());
    emodel->PreRender();
    emodel->Render();
    emodel->PostRender();
    emodel->UnbindShader();
  }

  void GridRenderer3D::Submit(const Ref<Camera3D>& camera, const Cube& cube)
  {
    material->SetColor(Color{cube.color});
    mesh->SetEnableCulling(false);
    emodel->SetScale(Vec3f(1, 1, 1));
    emodel->SetPosition(cube.GetPosition());
    emodel->SetRotation(Vec3f(0, 0, 0));
    emodel->UpdateTransformation();
    emodel->GetMaterial()->Bind();
    camera->SetShaderUniforms(emodel->GetMaterial()->GetShader());
    BindMatrices(emodel->GetMaterial()->GetShader(), camera);
    emodel->GetMesh()->Bind();

    emodel->GetMaterial()->GetShader()->SetUniformMat4("uTransformationMatrix", emodel->GetTransformationMatrix());
    emodel->GetMesh()->Render();
    emodel->GetMesh()->Unbind();
    emodel->GetMaterial()->GetShader()->Disable();
    //emodel->PreRender(this, camera);
    //emodel->Render(this, camera);
    //emodel->PostRender(this, camera);
  }

  void GridRenderer3D::DrawLineCube(const Ref<Camera3D>& camera, const Vec3f& pos, const Vec3f& size, const Vec4f& color)
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

  void GridRenderer3D::DrawLine(const Ref<Camera3D>& camera, const Vec3f& start, const Vec3f& end, const Vec4f& color)
  {
    lineShader->Enable();
    camera->SetShaderUniforms(lineShader);
    lineShader->SetUniform4f("uMaterialColor", color);

    vbo->Enable();
    Vec3f* buffer = (Vec3f*)vbo->MapBuffer();

    buffer[0] = start;
    buffer[1] = end;

    vbo->UnmapBuffer();
    vbo->Disable();

    vao->Enable();
    vao->Render(DrawType::LINES, 2);
    vao->Disable();

    lineShader->Disable();
  }
}
