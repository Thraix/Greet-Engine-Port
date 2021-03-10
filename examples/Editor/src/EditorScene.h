#include <Greet.h>

class EditorScene : public Greet::ECSScene
{
  Greet::Ref<Greet::Shader> lineShader;
  Greet::Ref<Greet::Mesh> lineMesh;

  Greet::Entity selectedEntity;

  public:
    EditorScene();

    void OnEvent(Greet::Event& event) override;
    void Render3D() const override;
};
