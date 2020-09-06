#pragma once

#include <initializer_list>

#include <graphics/gui/Component.h>
#include <graphics/gui/Text.h>
#include <graphics/gui/TreeNode.h>

namespace Greet
{


  class TreeView : public Component
  {
    private:
      REGISTER_COMPONENT_DECLARATION(TreeView);
      friend class TreeNode;
    private:
      Text text;
      TreeNode* tree;
      float spacing = 5;
      float indentSize = 20;

      TreeNode* hovered = nullptr;
      TreeNode* selected = nullptr;
    public:
      TreeView(const XMLObject& object, Component* parent);

      void SetTreeNode(TreeNode* node);

      virtual void Render(GUIRenderer* renderer) const override;

      virtual void OnEvent(Event& event, const Vec2& componentPos) override;
      virtual void MouseExited() override;

      virtual float GetWrapWidth() const override;
      virtual float GetWrapHeight() const override;
  };
}

