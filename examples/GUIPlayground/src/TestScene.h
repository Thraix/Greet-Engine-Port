#pragma once

#include <graphics/layers/Scene.h>
#include <graphics/renderers/BatchRenderer.h>
#include <graphics/shaders/ShaderFactory.h>
#include <graphics/Window.h>
#include <utils/LogUtils.h>
#include <utils/ColorUtils.h>

enum Type
{
  Pixel, Weight, Wrap
};

struct Size
{
  int value;
  Type type;
  int size;
};

struct Comp
{
  int x;
  int y;
  Size width;
  Size height;
  uint color;
  Comp* parent;
  std::string name;

  Comp(const std::string& name, Size width, Size height, uint color)
    : x{0}, y{0}, width{width}, height{height}, color{color}, name{name}
  {}

  virtual void Render(Greet::BatchRenderer* renderer, int xOffset, int yOffset) const
  {
    /* Greet::Log::Info(GetName(), " pos: ", xOffset + x, ", ", yOffset + y); */
    /* Greet::Log::Info(GetName(), " size: ", width.size, ", ", height.size); */
    renderer->FillRect({(float)(x + xOffset), (float)(y + yOffset)}, {(float)width.size, (float)height.size}, color);
  }

  virtual void Measure(int parentWidth, int parentHeight)
  {
    if(width.type == Pixel)
      width.size = width.value;
    else if(width.type == Weight)
      width.size = parentWidth * width.value;
    else
      width.size = GetWrapWidth();

    if(height.type == Pixel)
      height.size = height.value;
    else if(height.type == Weight)
      height.size = parentHeight * height.value;
    else
      height.size = GetWrapHeight();
  }

  void SetWidthValue(int value)
  {
    width.value = value;
    /* InvalidateWidth(); */
  }

  void InvalidateWidth()
  {
    if(width.size == 0)
      return;
    HandleInvalidateWidth();
  }

  void InvalidateHeight()
  {
    if(height.size == 0)
      return;
    HandleInvalidateHeight();
  }

  virtual void HandleInvalidateWidth()
  {
    width.size = 0;
    if(parent)
    {
      if(parent->width.type == Type::Wrap)
      {
        parent->InvalidateWidth();
      }
    }
  }

  virtual void HandleInvalidateHeight()
  {
    height.size = 0;
    if(parent)
    {
      if(parent->height.type == Type::Wrap)
      {
        parent->InvalidateHeight();
      }
    }
  }

  virtual int GetWrapWidth() { return 100; }
  virtual int GetWrapHeight() { return 100; }

  std::string GetName() const { return name; }
};

struct Lab : public Comp
{
  int length;

  Lab(const std::string& name, Size width, Size height, uint color, int length)
    : Comp{name, width, height, color}, length{length}
  {}

  void Render(Greet::BatchRenderer* renderer, int xOffset, int yOffset) const override
  {
    Comp::Render(renderer, xOffset, yOffset);
    int xPos = 0;
    int yPos = 0;
    for(int i = 0; i < length; i++)
    {
      uint textColor = Greet::ColorUtils::Vec4ToColorHex(Greet::ColorUtils::HSVtoRGB(1.0 * i / length, 1, 1, 1));
      renderer->FillRect({(float)(x + xPos + xOffset), (float)(y + yPos + yOffset)}, {20.0f, 20.0f}, textColor);
      xPos += 30;
      if(xPos + 20 > width.size)
      {
        xPos = 0;
        yPos += 30;
      }
    }
  }

  int GetWrapWidth() override
  {
    return length * 20 + (length - 1) * 10;
  }

  int GetWrapHeight() override
  {
    int w = 0;
    int lines = 1;
    for(int i = 0; i < length; i++)
    {
      w += 30;
      if(w + 20 > width.size)
      {
        lines++;
        w = 0;
      }
    }
    if(w == 0 && lines > 1)
      lines--;
    return lines * 20 + (lines - 1) * 10;
  }

  void HandleInvalidateWidth() override
  {
    Comp::HandleInvalidateWidth();
    if(height.type == Type::Wrap)
      InvalidateHeight();
  }

};


struct Cont : Comp
{
  std::vector<Comp*> children;
  bool vertical;

  Cont(const std::string& name, Size width, Size height, uint color, bool vertical)
    : Comp{name, width, height, color}, vertical{vertical}
  {}

  void Render(Greet::BatchRenderer* renderer, int xOffset, int yOffset) const override
  {
    Comp::Render(renderer, xOffset, yOffset);
    for(auto&& child : children)
      child->Render(renderer, xOffset + x, yOffset + y);
  }

  void MeasureChildren(int weightWidth, int weightHeight)
  {
    int xPos = 10; // 10 = padding
    int yPos = 10; // 10 = padding
    int spacings = std::max((int)children.size() - 1, 0);
    int totalWidth = 20 + (vertical ? 0 : 10 * spacings);
    int totalHeight = 20 + (!vertical ? 0 : 10 * spacings);
    for(auto&& child : children)
    {
      child->Measure(0, 0);
      child->x = xPos;
      child->y = yPos;
      if(vertical)
        yPos += child->height.size + 10; // 10 = spacing
      else
        xPos += child->width.size + 10; // 10 = spacing
      if(child->width.type != Type::Weight && !vertical)
        totalWidth += child->width.size;
      if(child->height.type != Type::Weight && vertical)
        totalHeight += child->height.size;
    }

    weightWidth = weightWidth == 0 ? 1 : weightWidth;
    weightHeight = weightHeight == 0 ? 1 : weightHeight;
    totalWidth = totalWidth > width.size ? width.size : totalWidth;
    totalHeight = totalHeight > height.size ? height.size : totalHeight;

    xPos = 10; // 10 = padding
    yPos = 10; // 10 = padding
    int mod = 0;
    for(auto&& child : children)
    {
      if(vertical)
      {
        child->Measure((width.size - totalWidth), (height.size - totalHeight + mod) / weightHeight);
      }
      else
      {
        child->Measure((width.size - totalWidth + mod) / weightWidth, (height.size - totalHeight));
      }
      child->x = xPos;
      child->y = yPos;
      if(vertical)
        yPos += child->height.size + 10; // 10 = spacing
      else
        xPos += child->width.size + 10; // 10 = spacing
    }
  }

  virtual void Measure(int parentWidth, int parentHeight) override
  {
    bool childHasWidthWeight = false;
    bool childHasHeightWeight = false;
    int totalWeightWidth = 0;
    int totalWeightHeight = 0;
    for(auto&& child : children)
    {
      if(child->width.type == Type::Weight)
      {
        totalWeightWidth += child->width.value;
        childHasWidthWeight = true;
      }
      if(child->height.type == Type::Weight)
      {
        totalWeightHeight += child->height.value;
        childHasWidthWeight = true;
      }
    }
    Comp::Measure(parentWidth, parentHeight);

    MeasureChildren(totalWeightWidth, totalWeightHeight);

    bool remeasureChildren = true;
    while(remeasureChildren)
    {
      remeasureChildren = false;
      if(width.type == Wrap)
      {
        int wrapSize = GetWrapWidth();
        if(wrapSize != width.size)
        {
          width.size = wrapSize;
          if(childHasWidthWeight)
            remeasureChildren = true;
        }
      }
      if(height.type == Wrap)
      {
        int wrapSize = GetWrapHeight();
        if(wrapSize != height.size)
        {
          height.size = wrapSize;
          if(childHasHeightWeight)
            remeasureChildren = true;
        }
      }
      if(remeasureChildren)
      {
        MeasureChildren(totalWeightWidth, totalWeightHeight);
      }
    }
  }

  virtual int GetWrapWidth() override
  {
    int width = 0;
    if(vertical)
    {
      for(auto&& child : children)
      {
        if(child->width.type != Type::Weight)
          width = std::max(width, child->width.size);
      }
    }
    else
    {
      width = 10 * (children.size() - 1);
      for(auto&& child : children)
      {
        if(child->width.type != Type::Weight)
          width += child->width.size;
      }
    }
    return width + 20;
  }

  virtual int GetWrapHeight() override
  {
    int height = 0;
    if(vertical)
    {
      height = 10 * (children.size() - 1);
      for(auto&& child : children)
      {
        if(child->height.type != Type::Weight)
          height += child->height.size;
      }
    }
    else
    {
      for(auto&& child : children)
      {
        if(child->height.type != Type::Weight)
          height = std::max(height, child->height.size);
      }
    }
    return height + 20;
  }

  void HandleInvalidateWidth() override
  {
    Comp::HandleInvalidateWidth();
    for(auto&& child : children)
    {
      if(child->width.type == Type::Weight)
      {
        child->InvalidateWidth();
      }
    }
  }

  void HandleInvalidateHeight() override
  {
    Comp::HandleInvalidateHeight();
    for(auto&& child : children)
    {
      if(child->height.type == Type::Weight)
      {
        child->InvalidateHeight();
      }
    }
  }

  void AddComp(Comp* comp)
  {
    children.push_back(comp);
    comp->parent = this;
  }

};

struct TestScene : public Greet::Scene
{
  Greet::BatchRenderer* renderer;
  Cont cont = {"Master", {1, Type::Wrap}, {1, Type::Wrap}, 0xffffffff, true};
  Lab* lab;
  float  time = 0;

  TestScene()
  {
    renderer = new Greet::BatchRenderer(Greet::ShaderFactory::Shader2D());

    Cont* cont1 = new Cont{"Top Container", {1, Type::Weight}, {40, Type::Pixel}, 0xffaaaaaa, true};
    Cont* cont2 = new Cont{"Mid Container", {1, Type::Wrap}, {1, Type::Wrap}, 0xffaaaaaa, true};
    Cont* cont3 = new Cont{"Bot Container", {1, Type::Weight}, {40, Type::Pixel}, 0xffaaaaaa, false};
    Cont* cont4 = new Cont{"Bot Left Container", {1, Type::Weight}, {1, Type::Weight}, 0xffffffff, true};
    Cont* cont5 = new Cont{"Bot Right Container", {1, Type::Weight}, {1, Type::Weight}, 0xffffffff, true};
    lab = new Lab{"Top Label", {380, Type::Pixel}, {1, Type::Wrap}, 0xffffffff, 26};
    Lab* lab2 = new Lab{"Bot Label", {1, Type::Weight}, {1, Type::Wrap}, 0xffffffff, 15};
    cont2->AddComp(lab);
    cont2->AddComp(lab2);
    cont3->AddComp(cont4);
    cont3->AddComp(cont5);
    cont.AddComp(cont1);
    cont.AddComp(cont2);
    cont.AddComp(cont3);
    cont.Measure(400, 400);
  }

  virtual void Render() const override
  {
    renderer->Begin();
    renderer->GetShader()->SetUniformMat3("projectionMatrix", Greet::Mat3::OrthographicViewport());
    cont.Render(renderer, 0, 0);
    renderer->End();
    renderer->Flush();
  }

  virtual void Update(float timeElapsed) override
  {
    time += timeElapsed;
    lab->SetWidthValue(400 + 100 * sin(time));
    cont.Measure(400, 400);
  }
};
