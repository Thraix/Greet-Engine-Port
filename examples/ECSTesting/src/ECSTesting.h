#pragma once

#include <Greet.h>

#include "ECSScene.h"

class ECSTesting : public Greet::App
{
  private:
    Greet::GUIScene* gui;
    Greet::Ref<ECSScene> scene;
  public:
    ECSTesting();

    ~ECSTesting();

    void Init() override;

    void Tick() override;
    void Update(float timeElapsed) override;
    void Render() override;
    void OnEvent(Greet::Event& event) override;
};
