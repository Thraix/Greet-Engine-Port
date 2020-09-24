#pragma once

#include <Greet.h>

class ECSTesting : public Greet::App
{
  private:
    Greet::Ref<Greet::GUIScene> gui;
    Greet::Ref<Greet::ECSScene> scene;
  public:
    ECSTesting();

    ~ECSTesting();

    void Init() override;

    void Tick() override;
    void Update(float timeElapsed) override;
    void Render() override;
    void OnEvent(Greet::Event& event) override;
};
