#include "ShaderFactory.h"

#include <graphics/shaders/Shader.h>


namespace Greet { namespace ShaderFactory {


  Ref<Shader> LEDShader()
  {
    return Shader::FromSource(default_shader_vert, led_shader_frag);
  }

  Ref<Shader> DefaultShader()
  {
    return Shader::FromSource(default_shader_vert, default_shader_frag);
  }

  Ref<Shader> BasicLightShader()
  {
    return Shader::FromSource(default_shader_vert, basic_light_shader_frag);
  }
  Ref<Shader> DebugShader()
  {
    return Shader::FromSource(default_shader_vert, debug_shader_frag);
  }

  Ref<Shader> SkyboxShader()
  {
    return Shader::FromSource(skybox_shader_vert, skybox_shader_frag);
  }
}}
