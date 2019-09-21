#include "ShaderFactory.h"

#include <graphics/shaders/Shader.h>


namespace Greet {

  const char* ShaderFactory::default_shader_vert =
#include "default2dshader.vert"
    ;

  const char* ShaderFactory::default_shader_frag =
#include "default2dshader.frag"
    ;

  const char* ShaderFactory::debug_shader_frag =
#include "debugshader.frag"
    ;

  const char* ShaderFactory::led_shader_frag =
#include "ledshader.frag"
    ;

  const char* ShaderFactory::basic_light_shader_frag =
#include "basiclightshader.frag"
    ;

  const char* ShaderFactory::skybox_shader_vert =
#include "skyboxshader.vert"
    ;

  const char* ShaderFactory::skybox_shader_frag =
#include "skyboxshader.frag"
    ;

  Ref<Shader> ShaderFactory::LEDShader()
  {
    return Shader::FromSource(default_shader_vert, led_shader_frag);
  }

  Ref<Shader> ShaderFactory::DefaultShader()
  {
    return Shader::FromSource(default_shader_vert, default_shader_frag);
  }

  Ref<Shader> ShaderFactory::BasicLightShader()
  {
    return Shader::FromSource(default_shader_vert, basic_light_shader_frag);
  }
  Ref<Shader> ShaderFactory::DebugShader()
  {
    return Shader::FromSource(default_shader_vert, debug_shader_frag);
  }

  Ref<Shader> ShaderFactory::SkyboxShader()
  {
    return Shader::FromSource(skybox_shader_vert, skybox_shader_frag);
  }
}
