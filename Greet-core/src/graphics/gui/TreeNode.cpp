#include "TreeNode.h"

#include <graphics/gui/TreeView.h>

#include <algorithm>

namespace Greet
{
  TreeNode::TreeNode(const std::string& name)
    : name{name}
  {}

  TreeNode::TreeNode(const std::string& name, const std::initializer_list<TreeNode>& nodes)
    : name{name}, childNodes{nodes}
  {
    for(auto&& node : childNodes)
      node.parent = this;

    // TODO: Remove these testing cases before merging
    if(name == "Entity2")
      open = false;
    if(name == "Translation")
      open = false;

  }

  void TreeNode::Render(GUIRenderer* renderer, const TreeView& view) const
  {
    float offset = 0.0f;
    Render(renderer, offset, 0, view);
  }

  void TreeNode::Render(GUIRenderer* renderer, float& offset, int indent, const TreeView& view) const
  {
    if(!IsOpen())
      return;

    float indentOffset = indent * view.indentSize;

    for(auto&& node : childNodes)
    {
      if(node.hovered)
        renderer->DrawRect({indentOffset, offset}, {view.GetWidth() - indentOffset, (float)view.text.font.GetSize()}, Color(0xff323232), false);
      renderer->PushTranslation(Vec2{indentOffset, offset});
      float width = view.text.font.GetWidthOfText(name);
      renderer->DrawText(node.name, {0, (float)view.text.font.GetBaselineOffset()}, view.text.font, view.text.color, false);
      renderer->PopTranslation();
      offset += view.text.font.GetSize() + view.spacing;
      if(open)
        node.Render(renderer, offset, indent+1, view);
    }
  }

  TreeNode* TreeNode::GetTreeNodeAt(const Vec2& position, const TreeView& view)
  {
    float width = GetWidth(view);
    float height = GetHeight(view);
    if(!Utils::IsInside(position, {0, 0}, {width, height}))
      return nullptr;
    float yPos = position.y;
    return GetTreeNodeAt(yPos, view);
  }

  TreeNode* TreeNode::GetTreeNodeAt(float& yPos, const TreeView& view)
  {
    if(!IsOpen())
      return nullptr;

    float offset = 0.0f;
    for(auto&& node : childNodes)
    {
      if(yPos < view.text.font.GetSize())
        return &node;
      yPos -= view.spacing + view.text.font.GetSize();
      TreeNode* n = node.GetTreeNodeAt(yPos, view);
      if(n != nullptr)
        return n;
    }
    return nullptr;
  }

  void TreeNode::AddChildNode(TreeNode&& node)
  {
    childNodes.push_back(node);
    childNodes.back().parent = this;
    MarkDirty();
  }

  float TreeNode::GetWidth(const TreeView& view) const
  {
    return GetWidth(0, view);
  }

  float TreeNode::GetWidth(int indent, const TreeView& view) const
  {
    if(!IsOpen())
      return 0.0f;

    float width = 0;
    for(auto&& node : childNodes)
    {
      width = std::max(width, (float)view.text.font.GetWidthOfText(node.name) + indent * view.indentSize);
      width = std::max(width, node.GetWidth(indent + 1, view));
    }
    return width;
  }

  float TreeNode::GetHeight(const TreeView& view) const
  {
    if(!IsOpen())
      return 0.0f;

    float height = 0;
    for(auto&& node : childNodes)
    {
      height += node.GetHeight(view) + view.text.font.GetSize();
      height += view.spacing;
    }
    if(IsRoot())
      height -= view.spacing;
    return height;
  }


  void TreeNode::MarkDirty()
  {
    if(!IsRoot())
      parent->MarkDirty();
    else
      dirty = true;
  }
}
