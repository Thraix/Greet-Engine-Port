#include <Greet.h>

#include <graphics/renderers/GUIRenderer.h>
#include "keyboardcontrol.h"
#include "tree.h"
#include <random>

using namespace Greet;

class Core : public App
{
  private:
    BatchRenderer3D* renderer3d;
    BatchRenderer3D* waterRenderer;
    Material* modelMaterial;
    Material* flatMaterial;
    Material* terrainMaterial;
    Material* waterMaterial;
    Material* stallMaterial;
    KeyboardControl* movement;
    KeyboardControl* rotation;

    EntityModel* stall;
    EntityModel* dragon;
    EntityModel* terrain;
    EntityModel* water;
    EntityModel* polygon;
    EntityModel* cube;
    EntityModel* sphere;
    EntityModel* tetrahedron;
    std::vector<EntityModel> models;
    FrameBufferObject* fbo;
    Light* light;


    float progressFloat;

    TPCamera* camera;
    Layer* scene3d;
    Layer* uilayer;
    Layer3D* layer3d;
    Button* button;
    Label* fps;
    Renderable2D* cursor;
    Renderable2D* driverTest;
    Renderable2D* fboScene;

  public:
    Core()
      : App("Best Game Ever", 960, 540)
    {
      SetFrameCap(0);
    }

    ~Core()
    {
      delete modelMaterial;
      delete stall;
      delete dragon;
      delete terrain;
      delete water;
      delete cube;
      delete sphere;
      delete tetrahedron;
      delete renderer3d;
      delete waterRenderer;
      delete uilayer;
      delete movement;
      delete rotation;
      delete light;
    }

    void Init() override
    {
      TextureManager::Add("stall",Texture2D("res/textures/stallTexture.png"));
      TextureManager::Add("cursor",Texture2D("res/textures/cursor.png"));
      TextureManager::Add("mask",Texture2D("res/textures/mask.png"));
      TextureManager::Add("mask2",Texture2D("res/textures/mask2.png"));
      TextureManager::Add("skybox",CubeMap("res/textures/skybox.png"));
      TextureManager::Add("lensflare1",Texture2D("res/textures/lens_flare1.png"));
      TextureManager::Add("lensflare2",Texture2D("res/textures/lens_flare2.png"));
      TextureManager::Add("lensflare3",Texture2D("res/textures/lens_flare3.png"));
      TextureManager::Add("lensflare4",Texture2D("res/textures/lens_flare4.png"));
      FontManager::Add(new FontContainer("res/fonts/Anonymous Pro.ttf", "anonymous"));
      FontManager::Add(new FontContainer("res/fonts/Roboto-Thin.ttf", "roboto"));

      fbo = new FrameBufferObject(960,540);
      //camera = new TPCamera(vec3(-3.5, -7.8, 5.5), 18, 0.66, 38.5, 15, 80, 0, 0.8f); // Profile shot
      camera = new TPCamera(Mat4::ProjectionMatrix(Window::GetWidth()/ (float)Window::GetHeight(), 90, 0.1f,1000.0f), Vec3(0, 0, 0), 15, 0, 0, 0, 80, -0.8f, 0.8f);
      Skybox* skybox = new Skybox(TextureManager::Get3D("skybox"));
      renderer3d = new BatchRenderer3D();
      waterRenderer = new BatchRenderer3D();


      modelMaterial = new Material(Shader::FromFile("res/shaders/3dshader.shader"));
      flatMaterial = new Material(Shader::FromFile("res/shaders/flat3d.shader"));
      stallMaterial = new Material(Shader::FromFile("res/shaders/3dshader.shader"), TextureManager::Get2D("stall"));
      modelMaterial->SetSpecularStrength(0.25)->SetSpecularExponent(1)->SetDiffuseStrength(0.25);
      {
        terrainMaterial = new Material(Shader::FromFile("res/shaders/terrain.shader"));
        waterMaterial = new Material(Shader::FromFile("res/shaders/water.shader"));
        waterMaterial->SetSpecularExponent(50);
        waterMaterial->SetSpecularStrength(0.4);
        waterMaterial->GetShader().Enable();
        waterMaterial->GetShader().SetUniform1f("waterLevel",0.45f * 20.0f);
        waterMaterial->GetShader().Disable();

        terrainMaterial->SetSpecularStrength(0.5f);
        terrainMaterial->SetSpecularExponent(5.0f);

        uint gridWidth = 99;
        uint gridLength = 99;
        float* noise = Noise::GenNoise(gridWidth+1, gridWidth + 1,5,8, 8,0.5f);
        MeshData* gridMesh = MeshFactory::LowPolyGrid(0, 0, 0, gridWidth+1, gridLength+1, gridWidth, gridLength, noise,1);
        RecalcGrid(gridMesh, gridWidth, gridLength);
        terrain = new EntityModel(new Mesh(gridMesh), terrainMaterial, Vec3(0, -15, 0), Vec3(1.0f, 1.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f));
        gridMesh->RemoveAttribute(ATTRIBUTE_NORMAL);
        gridMesh->RemoveAttribute(ATTRIBUTE_COLOR);
        CalcGridVertexOffset(gridMesh);

        water = new EntityModel(new Mesh(gridMesh), waterMaterial, Vec3(0, -15, 0), Vec3(1.0f, 1.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f));
        delete gridMesh;
      }

      MeshData* polygonMesh = MeshFactory::Polygon(6, 10, MeshFactory::PolygonSizeFormat::SIDE_LENGTH);
      polygon = new EntityModel(new Mesh(polygonMesh), terrainMaterial, Vec3(0,1,0), Vec3(1,1,1), Vec3(0,0,0));


      MeshData* cubeMesh = MeshFactory::Cube(0,0,0,1,1,1);
      cube = new EntityModel(new Mesh(cubeMesh), modelMaterial, Vec3(1,0,0), Vec3(1, 1, 1), Vec3(0, 0, 0));
      delete cubeMesh;

      MeshData* sphereMeshData = MeshFactory::Sphere(Vec3(0,0,0), 0.5f, 20, 20);
      //sphereMeshData->LowPolify();
      Mesh* sphereMesh = new Mesh(sphereMeshData);
      //sphereMesh->SetEnableWireframe(true);
      sphere = new EntityModel(sphereMesh, modelMaterial, Vec3(0,0,0), Vec3(1, 1, 1), Vec3(0, 0, 0));
      delete sphereMeshData;

      MeshData* tetrahedronMesh = MeshFactory::Tetrahedron(0,0,0,10);
      tetrahedron = new EntityModel(new Mesh(tetrahedronMesh), modelMaterial, Vec3(30, 0, 10), Vec3(1, 1, 1), Vec3(0, 0, 0));
      delete tetrahedronMesh;

      MeshData* stallMeshData = OBJUtils::LoadObj("res/objs/stall.obj");
      //
      Mesh* stallMesh = new Mesh(stallMeshData);
      stallMaterial->SetSpecularStrength(0.1)->SetSpecularExponent(1);
      stall = new EntityModel(stallMesh, stallMaterial, Vec3(0.0f, 0.0f, -25), Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 0.0f, 0.0f));

      // MEMORY LEAK WITH MESHDATA
      MeshData* data = OBJUtils::LoadObj("res/objs/dragon.obj");
      Vec3* normals = new Vec3[data->GetVertexCount()];
      MeshFactory::CalculateNormals(data->GetVertices(), data->GetVertexCount(), data->GetIndices(), data->GetIndexCount(), normals);
      data->AddAttribute(new AttributeData<Vec3>(ATTRIBUTE_NORMAL, normals));
      data->LowPolify();
      data->WriteToFile("res/objs/dragon.gobj");
      Mesh* dragonMesh = new Mesh(data);
      dragon = new EntityModel(dragonMesh, flatMaterial, Vec3(20.0f, 0.0f, -25), Vec3(1.0f, 1.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f));

      //Mesh* gridMesh = MeshFactory::cube(0,0,0,10,10,10);
      //gridMesh->setEnableCulling(false);
      //MaterialModel* gridModelMaterial = new MaterialModel(gridMesh, *modelMaterial);
      //grid = new EntityModel(*gridModelMaterial, vec3(0, 0, 0), vec3(1, 1, 1), vec3(0, 0, 0));

      //for (uint i = 0;i < 2000;i++)
      //{
      //	models.push_back(EntityModel(*modelModelMaterial, vec3(random()*100, random() * 100, random() * 100), vec3(1.0f, 1.0f, 1.0f), vec3(random() * 360, random() * 360, random() * 360)));
      //}

      light = new Light(Vec3(10, 20, 10), 0xffffffff);
      const Shader& modelShader = modelMaterial->GetShader();
      modelShader.Enable();
      light->SetToUniform(modelShader, "light");
      modelShader.Disable();
      const Shader& flatShader = flatMaterial->GetShader();
      flatShader.Enable();
      light->SetToUniform(flatShader, "light");
      flatShader.Disable();

      uilayer = new Layer(new BatchRenderer(), ShaderFactory::DefaultShader(), Mat3::Orthographic(0.0f, (float)Window::GetWidth(), 0.0f, (float)Window::GetHeight()));
      Vec4 colorPink = ColorUtils::GetMaterialColorAsHSV(300 /360.0f, 3);
      cursor = new Renderable2D(Vec2(0,0),Vec2(32,32),0xffffffff, new Sprite(TextureManager::Get2D("cursor")), new Sprite(TextureManager::Get2D("mask")));
      uilayer->Add(cursor);

      renderer3d->Submit(stall);
      //renderer3d->Submit(dragon);
      renderer3d->Submit(terrain);
      renderer3d->Submit(sphere);
      renderer3d->Submit(cube);
      waterRenderer->Submit(water);
      //renderer3d->Submit(polygon);
      //renderer3d->Submit(tetrahedron);
      //for (uint i = 0;i < 2000;i++)
      //{
      //	renderer3d->submit(&models[i]);
      //}
      movement = new KeyboardControl(GLFW_KEY_D,GLFW_KEY_A,GLFW_KEY_S,GLFW_KEY_W,0.5f);
      rotation = new KeyboardControl(GLFW_KEY_DOWN,GLFW_KEY_UP,GLFW_KEY_RIGHT,GLFW_KEY_LEFT,2);
      //Tree t(renderer3d,0,0,0);
      uint pos = 0;
      //		Log::info(JSONLoader::isNumber("0.1234s",pos));
      RenderEngine::Add2DScene(uilayer, "uilayer");
      layer3d = new Layer3D(camera, skybox);
      layer3d->AddRenderer(renderer3d);
      layer3d->AddRenderer(waterRenderer);
      RenderEngine::Add3DScene(layer3d, "3dWorld");
    }

    void RecalcPositions(Vec3* vertex)
    {
      float y = vertex->y;
      if (y < 0.45)
      {
        //y = 0.45f + Noise::PRNG(vertex->x, vertex->z)*0.01f;// + 0.03f*(rand() / (float)RAND_MAX - 0.5f);
      }
      else if (y < 0.48)
      {

      }
      else if (y < 0.58)
      {

      }
      else if (y < 0.65)
      {
        y = (pow(y - 0.58, 0.6) + 0.58);
      }
      else
      {
        y = (pow(y - 0.58, 0.6) + 0.58);
      }
      vertex->y = y * 20.0;
    }

    void RecalcColors(const Vec3& v1, const Vec3& v2, const Vec3& v3, uint* color)
    {
      float y = (v1.y + v2.y + v3.y) / 3.0f / 20.0f;
      if (y < 0.45+0.01f)
      {
        uint blue = (uint)(pow(1, 4.0f) * 255);
        blue = blue > 255 ? 255 : blue;
        *color = 0xff000000 | ((blue / 2) << 16) | ((uint)(blue * 0.9) << 8) | blue;
        *color = 0xffF0E5A5;
      }
      else if (y < 0.48)
      {
        *color = 0xffF0E5A5;
      }
      else if (y < 0.58)
      {
        *color = 0xff7CD663;
      }
      else if (y < pow(0.07, 0.6) + 0.58)
      {
        *color = 0xffB5B0A8;
      }
      else
      {
        *color = 0xffDCF2F2;
      }
    }

    void CalcGridVertexOffset(MeshData* data)
    {
      Vec3* vertices = data->GetVertices();
      uint vertexCount = data->GetVertexCount();
      uint indexCount = data->GetIndexCount();
      uint* indices = data->GetIndices();

      byte* offsets = new byte[4 * vertexCount];
      for (int i = 0;i < indexCount;i+=3)
      {
        Vec3 v1 = vertices[indices[i+1]] - vertices[indices[i]];
        Vec3 v2 = vertices[indices[i+2]] - vertices[indices[i]];
        offsets[indices[i]*4] = round(v1.x);
        offsets[indices[i]*4 + 1] = round(v1.z);
        offsets[indices[i]*4 + 2] = round(v2.x);
        offsets[indices[i]*4 + 3] = round(v2.z);
      }
      data->AddAttribute(new AttributeData<byte>(AttributeDefaults(4, 4, 4 * sizeof(byte), GL_BYTE,GL_FALSE), offsets));
    }

    void RecalcGrid(MeshData* data, uint gridWidth, uint gridLength)
    {
      uint* colors = new uint[data->GetVertexCount()];
      Vec3* vertices = data->GetVertices();
      uint indexCount = data->GetIndexCount();
      uint* indices = data->GetIndices();
      Vec3* normals = (Vec3*)data->GetAttribute(ATTRIBUTE_NORMAL)->data;
      for (int i = 0;i < indexCount;i+=3)
      {
        RecalcPositions(&vertices[indices[i]]);
      }
      uint index = MeshFactory::IndexGrid(gridWidth, gridLength - 1, gridWidth, gridLength);
      RecalcPositions(&vertices[MeshFactory::IndexGrid(gridWidth,gridLength-1,gridWidth,gridLength)]);
      index = MeshFactory::IndexGrid(0, gridLength, gridWidth, gridLength);
      RecalcPositions(&vertices[index]);
      for (int i = 0;i < indexCount;i += 3)
      {
        normals[indices[i]] = MeshFactory::CalculateNormal(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]);
        RecalcColors(vertices[indices[i]], vertices[indices[i+1]], vertices[indices[i+2]], &colors[indices[i]]);
      }
      data->AddAttribute(new AttributeData<uint>(ATTRIBUTE_COLOR, colors));
    }

    float Random()
    {
      return (float)((double)rand() / (RAND_MAX));
    }

    void Tick() override
    {
      std::string s = StringUtils::to_string(GetFPS()) + " fps | " + StringUtils::to_string(GetUPS())+ " ups";
      //fps->text = s;
      Window::SetTitle("Best Game Ever | " + s);
    }

    float hue = 0;
    Vec3 velocityPos;
    Vec3 velocityNeg;
    float waterTime = 0.0;

    void Update(float elapsedTime) override
    {
      waterTime += elapsedTime;
      waterMaterial->GetShader().Enable();
      waterMaterial->GetShader().SetUniform1f("time", waterTime);
      waterMaterial->GetShader().Disable();

      progressFloat++;
      if (progressFloat > 1000)
        progressFloat = 0;
      hue += elapsedTime / 3.0f;
      while (hue >= 1)
        hue--;
      cursor->m_color = ColorUtils::Vec3ToColorHex(ColorUtils::HSVtoRGB(hue, 1, 1,1.0));
      sphere->SetPosition(light->position);
    }

    void OnEvent(Event& event) override
    {
      if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
      {
        cursor->SetPosition(((MouseMoveEvent&)event).GetPosition());
      }
      else if(EVENT_IS_TYPE(event, EventType::KEY_RELEASE))
      {
        KeyReleaseEvent& e = (KeyReleaseEvent&)event;
        movement->onInput(e.GetButton(),false);
        rotation->onInput(e.GetButton(),false);
        if (e.GetButton() == GLFW_KEY_LEFT_SHIFT)
        {
          velocityNeg.y = 0;
        }
        if (e.GetButton() == GLFW_KEY_SPACE)
        { 
          velocityPos.y = 0;
        }
      }
      else if(EVENT_IS_TYPE(event, EventType::JOYSTICK_CONNECT))
      {
        Log::Info("Controller ", ((JoystickConnectEvent&)event).GetJoystick(), " connected!");
      }
      else if(EVENT_IS_TYPE(event, EventType::JOYSTICK_DISCONNECT))
      {
        Log::Info("Controller ", ((JoystickDisconnectEvent&)event).GetJoystick(), " disconnected!");
      }
      else if(EVENT_IS_TYPE(event, EventType::WINDOW_RESIZE))
      {
        uilayer->SetProjectionMatrix(Mat3::Orthographic(0,Window::GetWidth(),0,Window::GetHeight()));
      }
      else if(EVENT_IS_TYPE(event, EventType::KEY_PRESS))
      {
        KeyPressEvent& e = (KeyPressEvent&)event;
        if (e.GetButton() == GLFW_KEY_F5)
        {
          modelMaterial->SetShader(Shader::FromFile("res/shaders/3dshader.shader"));
          terrainMaterial->SetShader(Shader::FromFile("res/shaders/terrain.shader"));
          flatMaterial->SetShader(Shader::FromFile("res/shaders/flat3d.shader"));

          modelMaterial->GetShader().Enable();
          light->SetToUniform(modelMaterial->GetShader(), "light");
          modelMaterial->GetShader().Disable();
          flatMaterial->GetShader().Enable();
          light->SetToUniform(flatMaterial->GetShader(), "light");
          flatMaterial->GetShader().Disable();
          //terrainShader->enable();
          //l->setToUniform(terrainShader, "light");
          //terrainShader->disable();
        }
        if (e.GetButton() == GLFW_KEY_F1)
        {
          Log::Info("pos=", camera->GetPosition(), " height=", camera->GetHeight(), " distance=", camera->GetDistance(), ", rotation=", camera->GetRotation());
        }
        if (e.GetButton() == GLFW_KEY_F2)
        {
          camera->SetPosition(Vec3(-3.5, -7.8, 5.5));
          camera->SetDistance(18);
          camera->SetHeight(0.66);
          camera->SetRotation(38.5);
        }
        if (e.GetButton() == GLFW_KEY_F10)
        {
          Utils::Screenshot(Window::GetWidth(), Window::GetHeight());
        }
        if (e.GetButton() == GLFW_KEY_X)
        {
          Vec3 p = layer3d->GetScreenCoordination(Vec3(0,0,0),Window::GetWidth(),Window::GetHeight());
        }
        movement->onInput(e.GetButton(),true);
        rotation->onInput(e.GetButton(),true);
        if (e.GetButton() == GLFW_KEY_LEFT_SHIFT)
        {
          velocityNeg.y = 0.2;
        }
        if (e.GetButton() == GLFW_KEY_SPACE)
        {
          velocityPos.y = 0.2;
        }
      }
      else if(EVENT_IS_TYPE(event, EventType::JOYSTICK_PRESS))
      {
        JoystickPressEvent& e = (JoystickPressEvent&)event;
        Log::Info("Joystick button pressed joy=", e.GetJoystick(), " button=", e.GetButton());
      }
    }

    bool screenshot = false;
    void Render() override
    {
    }
};

#include <fstream>
int main()
{
  /*
     uint pos = 0;
     uint lastPos = pos;
     JSONObject obj = JSONLoader::loadJSON("test.txt");
     Log::info("object1",obj.hasKey("object1") ? "true" : "false");
     Log::info("object1.string1", obj.getObject("object1").getValue("string1"));
     Log::info("object1.float", obj.getObject("object1").getValueAsFloat("float"));
     Log::info("object1.null", obj.getObject("object1").isNull("null") ? "true" : "false");
     Log::info("object1.true", obj.getObject("object1").getValueAsBool("true") ? "true" : "false");
     Log::info("object1.false", obj.getObject("object1").getValueAsBool("false") ? "true" : "false");
     Log::info("string2", obj.getValueAsFloat("string2"));
     std::ofstream file("save.txt");
     file << obj;
     file.close();
     system("pause");
     */
  Core game;
  game.Start();
}
