#pragma once

#include <graphics/renderers/GUIRenderer.h>
#include <graphics/gui/Text.h>
#include <graphics/gui/TLBR.h>

#include <vector>
#include <string>

namespace Greet
{
  class TreeView;

  class TreeNode
  {
    friend class TreeView;
    struct Styling
    {
      ////////////////////
      // Styling variables
      TLBR padding;
      TLBR border;
      Color backgroundColor;
      Color borderColor;
      float backgroundRadius;
      float borderRadius;
      int backgroundRoundedPrecision;
      int borderRoundedPrecision;
    };

    private:
      std::vector<TreeNode> childNodes;
      TreeNode* parent = nullptr;
      std::string name;
      bool open = true;
      bool selected = false;
      bool hovered = false;
      bool hoverFlowController = false;

      bool dirty = false;
      Styling styling;

    public:

      TreeNode(const std::string& name);
      TreeNode(const std::string& name, const std::initializer_list<TreeNode>& nodes);

      TreeNode(const TreeNode& node);
      TreeNode(TreeNode&& node);

      TreeNode& operator=(const TreeNode& node);
      TreeNode& operator=(TreeNode&& node);

      void Render(GUIRenderer* renderer, const TreeView& view) const;

      void AddChildNode(TreeNode&& node);

      std::pair<TreeNode*, bool> GetTreeNodeAt(const Vec2& position, const TreeView& view);

      float GetWidth(const TreeView& view) const;
      float GetHeight(const TreeView& view) const;

      void MarkDirty();
      void UnmarkDirty();

      bool IsLeaf() const { return childNodes.empty(); }
      bool IsRoot() const { return parent == nullptr; }
      bool IsOpen() const { return open && !IsLeaf(); }
      const std::string& GetName() const { return name; }

      void ToggleOpen(TreeView& view);
      void SetHovered(bool hover, const TreeView& view, bool hoverFlowController);
      void SetSelected(bool hover, TreeView& view);

      std::vector<TreeNode>::iterator begin() { return childNodes.begin(); }
      std::vector<TreeNode>::iterator end() { return childNodes.end(); }
      std::vector<TreeNode>::const_iterator begin() const { return childNodes.cbegin(); }
      std::vector<TreeNode>::const_iterator end() const { return childNodes.cend(); }

    private:
      void Render(GUIRenderer* renderer, float& offset, int indent, const TreeView& view) const;
      void RenderFlowController(GUIRenderer* renderer, const TreeView& view) const;

      std::pair<TreeNode*, bool> GetTreeNodeAt(Vec2& position, int indent, const TreeView& view);
      float GetWidth(int indent, const TreeView& view) const;
      float GetFlowControllerWidth(const TreeView& view) const;

      void SetStyling(const std::string& styleName, const TreeView& view);
      void UpdateStyling(const TreeView& view);
  };
}

