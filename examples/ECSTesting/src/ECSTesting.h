#pragma once

#include <Greet.h>

class ECSTesting : public Greet::App
{
  private:
    Greet::SceneView* sceneView;
  public:
    ECSTesting();

    ~ECSTesting();

    void Init() override;
    void Destruct() override;

    void Tick() override;
    void Update(float timeElapsed) override;
    void Render() override;
    void OnEvent(Greet::Event& event) override;
};
