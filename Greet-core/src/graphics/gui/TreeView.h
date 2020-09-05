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

