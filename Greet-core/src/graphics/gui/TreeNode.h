#pragma once

#include <graphics/renderers/GUIRenderer.h>
#include <graphics/gui/Text.h>

#include <vector>
#include <string>

namespace Greet
{
  class TreeView;

  class TreeNode
  {
    friend class TreeView;

    private:
      std::vector<TreeNode> childNodes;
      TreeNode* parent = nullptr;
      std::string name;
      bool open = true;
      bool selected = false;
      bool hovered = false;


      bool dirty = false;
      // Sprite icon;
    public:

      TreeNode(const std::string& name);
      TreeNode(const std::string& name, const std::initializer_list<TreeNode>& nodes);

      void Render(GUIRenderer* renderer, const TreeView& view) const;

      void AddChildNode(TreeNode&& node);

      TreeNode* GetTreeNodeAt(const Vec2& position, const TreeView& view);

      float GetWidth(const TreeView& view) const;
      float GetHeight(const TreeView& view) const;

      void MarkDirty();
      bool IsLeaf() const { return childNodes.empty(); }
      bool IsRoot() const { return parent == nullptr; }
      bool IsOpen() const { return open && !IsLeaf(); }

      std::vector<TreeNode>::iterator begin() { return childNodes.begin(); }
      std::vector<TreeNode>::iterator end() { return childNodes.end(); }
      std::vector<TreeNode>::const_iterator begin() const { return childNodes.cbegin(); }
      std::vector<TreeNode>::const_iterator end() const { return childNodes.cend(); }

    private:
      void Render(GUIRenderer* renderer, float& offset, int indent, const TreeView& view) const;
      TreeNode* GetTreeNodeAt(Vec2& position, int indent, const TreeView& view);
      float GetWidth(int indent, const TreeView& view) const;
  };
}

