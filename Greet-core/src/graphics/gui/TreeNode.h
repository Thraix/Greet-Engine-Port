#pragma once

#include <graphics/renderers/GUIRenderer.h>
#include <graphics/gui/Text.h>

#include <vector>
#include <string>

namespace Greet
{
  class TreeNode
  {
    friend class TreeView;
    private:
      std::vector<TreeNode> childNodes;
      TreeNode* parent = nullptr;
      std::string name;
      bool open = true;

      bool dirty = false;
      // Sprite icon;
    public:

      TreeNode(const std::string& name);
      TreeNode(const std::string& name, const std::initializer_list<TreeNode>& nodes);

      void Render(GUIRenderer* renderer, Text& text, float spacing, float indentSize) const;

      void AddChildNode(TreeNode&& node);

      float GetWidth(const Text& text, float indentSize) const;
      float GetHeight(const Text& text, float spacing) const;

      void MarkDirty();
      bool IsLeaf() const { return childNodes.empty(); }
      bool IsRoot() const { return parent == nullptr; }
      bool IsOpen() const { return open && !IsLeaf(); }

      std::vector<TreeNode>::iterator begin() { return childNodes.begin(); }
      std::vector<TreeNode>::iterator end() { return childNodes.end(); }
      std::vector<TreeNode>::const_iterator begin() const { return childNodes.cbegin(); }
      std::vector<TreeNode>::const_iterator end() const { return childNodes.cend(); }

    private:
      void Render(GUIRenderer* renderer, Text& text, float spacing, float indentSize, float& offset, int indent) const;
      float GetWidth(const Text& text, float indentSize, int indent) const;
  };
}

