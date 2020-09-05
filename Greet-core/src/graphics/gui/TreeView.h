#pragma once

#include <graphics/gui/Component.h>
#include <graphics/gui/Text.h>
#include <initializer_list>

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
      TreeNode(const std::string& name)
        : name{name}
      {}

      TreeNode(const std::string& name, const std::initializer_list<TreeNode>& nodes)
        : name{name}, childNodes{nodes}
      {
        for(auto&& node : childNodes)
          node.parent = this;
      }

      bool IsLeaf() const { return childNodes.empty(); }
      bool IsRoot() const { return parent == nullptr; }
      bool IsOpen() const { return open; }

      void MarkDirty()
      {
        if(!IsRoot())
          parent->MarkDirty();
        else
          dirty = true;
      }

      void AddChildNode(TreeNode&& node)
      {
        childNodes.push_back(node);
        childNodes.back().parent = this;
        MarkDirty();
      }

      void Render(GUIRenderer* renderer, Text& text, float spacing, float indentSize) const
      {
        float offset = 0.0f;
        Render(renderer, text, spacing, indentSize, offset, 0.0f);
      }

      void Render(GUIRenderer* renderer, Text& text, float spacing, float indentSize, float& offset, float indent) const
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

      std::vector<TreeNode>::iterator begin() { return childNodes.begin(); }
      std::vector<TreeNode>::iterator end() { return childNodes.end(); }
      std::vector<TreeNode>::const_iterator begin() const { return childNodes.cbegin(); }
      std::vector<TreeNode>::const_iterator end() const { return childNodes.cend(); }
  };
  class TreeView : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(TreeView);
  private:
    Text text;
    TreeNode* tree;
    float spacing = 5;
    float indentSize = 20;
  public:
    TreeView(const XMLObject& object, Component* parent);

    void SetTreeNode(TreeNode* node);

    virtual void Render(GUIRenderer* renderer) const override;

    virtual float GetWrapWidth() const override;
    virtual float GetWrapHeight() const override;
  };
}

