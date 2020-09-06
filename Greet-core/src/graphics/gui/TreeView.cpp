#include "TreeView.h"

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(TreeView);

  TreeView::TreeView(const XMLObject& object, Component* parent)
    : Component{object, parent}, text{""}, tree{new TreeNode{""}}
  {
    AddStyleVariables(StylingVariables{
        .colors={{"textColor", &text.color}},
        .floats={{"spacing", &spacing}, {"indentSize", &indentSize}},
        .fonts={{"font", &text.font}}
        });
    LoadStyles(object);
  }

  void TreeView::SetTreeNode(TreeNode* node)
  {
    delete tree;
    tree = node;
  }

  void TreeView::Render(GUIRenderer* renderer) const
  {
    tree->Render(renderer, *this);
  }

  void TreeView::OnEvent(Event& event, const Vec2& componentPos)
  {
    if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
      TreeNode* node = tree->GetTreeNodeAt(e.GetPosition() - componentPos, *this);
      if(node != nullptr)
      {
        if(hovered)
          hovered->hovered = false;
        hovered = node;
        hovered->hovered = true;
      }
    }
  }

  void TreeView::MouseExited()
  {
    if(hovered)
      hovered->hovered = false;
  }

  float TreeView::GetWrapWidth() const
  {
    return tree->GetWidth(*this);
  }

  float TreeView::GetWrapHeight() const
  {
    return tree->GetHeight(*this);
  }
}