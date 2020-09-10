#include "TreeView.h"

namespace Greet
{
  REGISTER_COMPONENT_DEFINITION(TreeView);

  TreeView::TreeView(const XMLObject& object, Component* parent)
    : Component{object, parent}, text{""}, tree{new TreeNode{""}}
  {
    AddStyleVariables(StylingVariables{
        .colors={
          {"textColor", &text.color},
          {"itemBackgroundColor", &itemBackgroundColor},
          {"itemBorderColor", &itemBorderColor}
        },
        .tlbrs={
          {"itemPadding", &itemPadding},
          {"itemBorder", &itemBorder}
        },
        .floats={
          {"spacing", &spacing},
          {"itemBackgroundRadius", &itemBackgroundRadius},
          {"itemBorderRadius", &itemBackgroundRadius}
        },
        .ints={
          {"itemBackgroundRoundedPrecision", &itemBackgroundRoundedPrecision},
          {"itemBorderRoundedPrecision", &itemBorderRoundedPrecision},
        },
        .fonts={{"font", &text.font}}
        });
    LoadStyles(object);
  }

  void TreeView::SetTreeNode(TreeNode* node)
  {
    delete tree;
    tree = node;
    tree->UpdateStyling(*this);
    Remeasure();
  }

  void TreeView::Render(GUIRenderer* renderer) const
  {
    renderer->PushTranslation(GetTotalPadding());
    renderer->PushViewport({0, 0}, GetContentSize(), false);
    tree->Render(renderer, *this);
    renderer->PopTranslation();
    renderer->PopViewport();
  }

  void TreeView::OnEvent(Event& event, const Vec2& componentPos)
  {
    if(EVENT_IS_TYPE(event, EventType::MOUSE_MOVE))
    {
      MouseMoveEvent& e = static_cast<MouseMoveEvent&>(event);
      TreeNode* node = tree->GetTreeNodeAt(e.GetPosition() - componentPos - GetTotalPadding(), *this);

      if(node != hovered)
      {
        if(hovered)
          hovered->SetHovered(false, *this);
        hovered = node;
        if(hovered)
          hovered->SetHovered(true, *this);
      }
    }
    else if(EVENT_IS_TYPE(event, EventType::MOUSE_PRESS))
    {
      MousePressEvent& e = static_cast<MousePressEvent&>(event);
      if(hovered)
        hovered->ToggleOpen(*this);
    }
    if(tree->dirty)
      Remeasure();
  }

  void TreeView::MouseExited()
  {
    if(hovered)
    {
      hovered->SetHovered(false, *this);
      hovered = nullptr;
    }
  }

  void TreeView::LoadFrameStyle(const MetaFile& metaFile)
  {
    Component::LoadFrameStyle(metaFile);
    tree->UpdateStyling(*this);
  }

  float TreeView::GetWrapWidth() const
  {
    return tree->GetWidth(*this);
  }

  float TreeView::GetWrapHeight() const
  {
    return tree->GetHeight(*this);
  }
}
