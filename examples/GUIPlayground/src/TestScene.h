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

  Comp(Size width, Size height, uint color)
    : x{0}, y{0}, width{width}, height{height}, color{color}
  {}

  virtual void Render(Greet::BatchRenderer* renderer, int xOffset, int yOffset) const
  {
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

  virtual int GetWrapWidth() { return 100; }
  virtual int GetWrapHeight() { return 100; }

  virtual std::string GetName() const { return "Comp"; }
};

struct Lab : public Comp
{
  int length;

  Lab(Size width, Size height, uint color, int length)
    : Comp{width, height, color}, length{length}
  {}

  void Render(Greet::BatchRenderer* renderer, int xOffset, int yOffset) const override
  {
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

  std::string GetName() const override { return "Label"; }
};


struct Cont : Comp
{
  std::vector<Comp*> children;
  bool vertical;

  Cont(Size width, Size height, uint color, bool vertical)
    : Comp{width, height, color}, vertical{vertical}
  {}

  void Render(Greet::BatchRenderer* renderer, int xOffset, int yOffset) const override
  {
    Comp::Render(renderer, xOffset, yOffset);
    for(auto&& child : children)
      child->Render(renderer, xOffset + x, yOffset + y);
  }

  virtual void Measure(int parentWidth, int parentHeight) override
  {
    bool childHasWidthWeight = false;
    bool childHasHeightWeight = false;
    Comp::Measure(parentWidth, parentHeight);
    int xPos = 10; // 10 = padding
    int yPos = 10; // 10 = padding
    for(auto&& child : children)
    {
      child->Measure(width.size - 20, height.size - 20);
      child->x = xPos;
      child->y = yPos;
      if(vertical)
        yPos += child->height.size + 10; // 10 = spacing
      else
        xPos += child->width.size + 10; // 10 = spacing
      if(child->width.type == Type::Weight)
        childHasWidthWeight = true;
      if(child->height.type == Type::Weight)
        childHasHeightWeight = true;
    }

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
        xPos = 10; // 10 = padding
        yPos = 10; // 10 = padding
        for(auto&& child : children)
        {
          child->Measure(width.size - 20, height.size - 20); // 20 = padding
          child->x = xPos;
          child->y = yPos;
          if(vertical)
            yPos += child->height.size + 10; // 10 = spacing
          else
            xPos += child->width.size + 10; // 10 = spacing
        }
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
        width = std::max(width, child->width.size);
      }
    }
    else
    {
      width = 10 * (children.size() - 1);
      for(auto&& child : children)
      {
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
        height += child->height.size;
      }
    }
    else
    {
      for(auto&& child : children)
      {
        height = std::max(height, child->height.size);
      }
    }
    return height + 20;
  }

  std::string GetName() const override { return "Container"; }
};

struct TestScene : public Greet::Scene
{
  Greet::BatchRenderer* renderer;
  Cont cont = {{1, Type::Wrap}, {1, Type::Wrap}, 0xffffffff, true};

  TestScene()
  {
    renderer = new Greet::BatchRenderer(Greet::ShaderFactory::Shader2D());

    Cont* cont1 = new Cont{{1, Type::Weight}, {40, Type::Pixel}, 0xff444444, true};
    Cont* cont2 = new Cont{{1, Type::Wrap}, {1, Type::Wrap}, 0xff444444, true};
    cont2->children.push_back(new Lab{{380, Type::Pixel}, {1, Type::Wrap}, 0xffffffff, 26});
    cont.children.push_back(cont1);
    cont.children.push_back(cont2);
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
  }
};
