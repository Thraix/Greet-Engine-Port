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
    float indentOffset = indent * view.indentSize;

    if(!IsRoot())
    {
      if(hovered)
        renderer->DrawRect({indentOffset, offset}, {view.GetWidth() - indentOffset, (float)view.text.font.GetSize()}, Color(0xff323232), false);
      renderer->PushTranslation(Vec2{indentOffset, offset});
      float width = view.text.font.GetWidthOfText(name);
      renderer->DrawText(name, {0, (float)view.text.font.GetBaselineOffset()}, view.text.font, view.text.color, false);
      renderer->PopTranslation();
      offset += view.text.font.GetSize() + view.spacing;
      indent++;
    }

    if(IsOpen())
    {
      for(auto&& node : childNodes)
      {
        node.Render(renderer, offset, indent, view);
      }
    }
  }

  TreeNode* TreeNode::GetTreeNodeAt(const Vec2& position, const TreeView& view)
  {
    float width = GetWidth(view);
    float height = GetHeight(view);
    if(!Utils::IsInside(position, {0, 0}, {width, height}))
      return nullptr;
    Vec2 pos = position;
    return GetTreeNodeAt(pos, 0, view);
  }

  TreeNode* TreeNode::GetTreeNodeAt(Vec2& position, int indent, const TreeView& view)
  {
    if(!IsRoot())
    {
      if(position.y >= 0 && position.y < view.text.font.GetSize())
      {
        position.y -= view.spacing + view.text.font.GetSize();
        if(position.x >= indent * view.indentSize)
          return this;
        return nullptr;
      }
      position.y -= view.spacing + view.text.font.GetSize();
      indent++;
    }

    if(IsOpen())
    {
      for(auto&& node : childNodes)
      {
        TreeNode* at = node.GetTreeNodeAt(position, indent, view);
        if(at)
          return at;
      }
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
    float width = 0;
    if(!IsRoot())
    {
      width = std::max(width, (float)view.text.font.GetWidthOfText(name) + indent * view.indentSize);
      indent++;
    }

    if(IsOpen())
    {
      for(auto&& node : childNodes)
      {
        width = std::max(width, node.GetWidth(indent, view));
      }
    }
    return width;
  }

  float TreeNode::GetHeight(const TreeView& view) const
  {
    float height = 0.0f;
    if(!IsRoot())
    {
      height += view.text.font.GetSize();
    }
    else
      height -= view.spacing;

    if(IsOpen())
    {
      for(auto&& node : childNodes)
      {
        height += node.GetHeight(view) + view.spacing;
      }
    }
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
