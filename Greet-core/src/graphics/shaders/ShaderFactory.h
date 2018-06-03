#pragma once

#include <graphics/shaders/Shader.h>

namespace Greet { namespace ShaderFactory {

	Shader LEDShader();
	Shader DefaultShader();
	Shader DebugShader();
	Shader BasicLightShader();
	Shader SkyboxShader();
}}