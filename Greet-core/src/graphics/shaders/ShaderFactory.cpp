#include "ShaderFactory.h"

#include <graphics/shaders/Shader.h>


namespace Greet { namespace ShaderFactory {


  Shader LEDShader()
  {
    return Shader::FromSource(default_shader_vert, led_shader_frag);
  }

  Shader DefaultShader()
  {
    return Shader::FromSource(default_shader_vert, default_shader_frag);
  }

  Shader BasicLightShader()
  {
    return Shader::FromSource(default_shader_vert, basic_light_shader_frag);
  }
  Shader DebugShader()
  {
    return Shader::FromSource(default_shader_vert, debug_shader_frag);
  }

  Shader SkyboxShader()
  {
    return Shader::FromSource(skybox_shader_vert, skybox_shader_frag);
  }
}}
