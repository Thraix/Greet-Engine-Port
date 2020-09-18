#include <Greet.h>

class Editor : public Greet::App
{
  private:
    Greet::GUIScene* gui;
    std::map<std::string, Greet::Ref<Greet::Scene>> scenes;
    Greet::SceneView* sceneView;
    Greet::TreeNode* sceneTree;
    Greet::TreeView* sceneTreeView;
  public:
    Editor();

    ~Editor();

    void Init() override;
    void SetupGUI(Greet::Frame* frame);

    void Tick() override;
    void Update(float timeElapsed) override;
    void Render() override;
    void OnEvent(Greet::Event& event) override;

    Greet::TreeNode* GetRootSceneTreeNode(Greet::TreeNode* node) const;
};
