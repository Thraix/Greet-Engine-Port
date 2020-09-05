#include "TreeNode.h"

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

  void TreeNode::Render(GUIRenderer* renderer, Text& text, float spacing, float indentSize) const
  {
    float offset = 0.0f;
    Render(renderer, text, spacing, indentSize, offset, 0);
  }

  void TreeNode::Render(GUIRenderer* renderer, Text& text, float spacing, float indentSize, float& offset, int indent) const
  {
    if(!IsOpen())
      return;
    for(auto&& node : childNodes)
    {
      text.str = node.name;
      renderer->PushTranslation(Vec2{indent * indentSize, offset});
      float wrap = text.GetWrapWidth();
      text.Render(renderer, {text.GetWrapWidth(), text.GetWrapHeight(wrap)});
      renderer->PopTranslation();
      offset += text.font.GetSize() + spacing;
      if(open)
        node.Render(renderer, text, spacing, indentSize, offset, indent+1);
    }
  }

  void TreeNode::AddChildNode(TreeNode&& node)
  {
    childNodes.push_back(node);
    childNodes.back().parent = this;
    MarkDirty();
  }

  float TreeNode::GetWidth(const Text& text, float indentSize) const
  {
    return GetWidth(text, indentSize, 0);
  }

  float TreeNode::GetWidth(const Text& text, float indentSize, int indent) const
  {
    if(!IsOpen())
      return 0.0f;

    float width = 0;
    for(auto&& node : childNodes)
    {
      width = std::max(width, (float)text.font.GetWidthOfText(node.name) + indent * indentSize);
      width = std::max(width, node.GetWidth(text, indentSize, indent+1));
    }
    return width;
  }

  float TreeNode::GetHeight(const Text& text, float spacing) const
  {
    if(!IsOpen())
      return 0.0f;

    float height = 0;
    for(auto&& node : childNodes)
    {
      height += node.GetHeight(text, spacing) + text.font.GetSize();
      height += spacing;
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
