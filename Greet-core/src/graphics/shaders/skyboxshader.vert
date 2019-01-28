R"skyboxshader(
#version 330 core

layout(location = 0) in vec3 position;
out vec3 vert_texCoord;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
  vec4 worldPos = vec4(viewMatrix *  vec4(position,0.0f));
	gl_Position = projectionMatrix * vec4(worldPos.xyz, 1.0);
	vert_texCoord = position;
}
)skyboxshader"
