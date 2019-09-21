#pragma once

#include <graphics/shaders/Shader.h>

namespace Greet {

  struct ShaderFactory
  {

    static const char* default_shader_vert;
    static const char* default_shader_frag;
    static const char* debug_shader_frag;
    static const char* led_shader_frag;
    static const char* basic_light_shader_frag;
    static const char* skybox_shader_vert;
    static const char* skybox_shader_frag;

    static Ref<Shader> LEDShader();
    static Ref<Shader> DefaultShader();
    static Ref<Shader> DebugShader();
    static Ref<Shader> BasicLightShader();
    static Ref<Shader> SkyboxShader();

  };
}
