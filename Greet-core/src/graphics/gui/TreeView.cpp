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
    Text tmpText = text;
    tree->Render(renderer, tmpText, spacing, indentSize);
  }

  float TreeView::GetWrapWidth() const
  {
    return tree->GetWidth(text, indentSize);
  }

  float TreeView::GetWrapHeight() const
  {
    return tree->GetHeight(text, spacing);
  }
}
