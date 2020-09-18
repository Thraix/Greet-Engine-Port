#include "Editor.h"

using namespace Greet;

Editor::Editor()
  : App{"Editor", 1440, 1440 * 9 / 16}
{
  SetFrameCap(60);
}

Editor::~Editor()
{
}

void Editor::Init()
{
  FontManager::Add("noto", FontContainer("res/fonts/noto.ttf"));
  gui = new GUIScene();
  Frame* frame = FrameFactory::GetFrame("res/guis/Editor.xml");
  gui->AddFrameQueued(frame);
  GlobalSceneManager::GetSceneManager().Add2DScene(gui, "gui");
  SetupGUI(frame);
}

void Editor::SetupGUI(Frame* frame)
{
  sceneView = frame->GetComponentByName<SceneView>("sceneView");
  Button* addSceneButton = frame->GetComponentByName<Button>("addSceneButton");
  Button* addEntityButton = frame->GetComponentByName<Button>("addEntityButton");
  sceneTreeView = frame->GetComponentByName<TreeView>("treeView");

  ASSERT(sceneView, "Could not load SceneView");
  ASSERT(addSceneButton, "Could not load Add Scene Button");
  ASSERT(addEntityButton, "Could not load Add Entity Button");
  ASSERT(sceneTreeView, "Could not load Scene TreeView");

  sceneTree = new TreeNode({});
  sceneTreeView->SetTreeNode(sceneTree);

  addSceneButton->SetOnPressCallback([&](Component* component)
  {
    std::string name = "Scene " + std::to_string(scenes.size());
    scenes.emplace(name, Ref<Scene>{new Scene()});
    sceneTree->AddChildNode(TreeNode{name});
  });

  addEntityButton->SetOnPressCallback([&](Component* component)
  {
    if(sceneTreeView->HasSelectedNode())
    {
      TreeNode* selected = sceneTreeView->GetSelectedNode();
      TreeNode* sceneNode = GetRootSceneTreeNode(selected);
      Ref<Scene> scene = scenes.find(sceneNode->GetName())->second;
      Entity entity = scene->AddEntity();
      selected->AddChildNode(TreeNode{"Entity: " + std::to_string(entity.GetID())});
    }
  });
}

TreeNode* Editor::GetRootSceneTreeNode(TreeNode* node) const
{
  TreeNode* current = node;
  while(!current->GetParent()->IsRoot())
  {
    current = current->GetParent();
  }
  return current;
}

void Editor::Tick()
{
}

void Editor::Render()
{
}

void Editor::Update(float timeElapsed)
{
}

void Editor::OnEvent(Event& event)
{
}
