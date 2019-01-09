#pragma once

#include <graphics/shaders/Shader.h>

namespace Greet { namespace ShaderFactory {

  static const char* default_shader_vert =
#include "default2dshader.vert"
    ;

  static const char* default_shader_frag =
#include "default2dshader.frag"
    ;

  static const char* debug_shader_frag =
#include "debugshader.frag"
    ;

  static const char* led_shader_frag =
#include "ledshader.frag"
    ;

  static const char* basic_light_shader_frag =
#include "basiclightshader.frag"
    ;

  static const char* skybox_shader_vert =
#include "skyboxshader.vert"
    ;

  static const char* skybox_shader_frag = 
#include "skyboxshader.frag"
    ;

  Shader LEDShader();
  Shader DefaultShader();
  Shader DebugShader();
  Shader BasicLightShader();
  Shader SkyboxShader();
}}
