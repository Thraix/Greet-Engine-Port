R"skyboxshader(
//vertex
#version 330 core

layout(location = 0) in vec3 aPosition;

out vec3 vTexCoord;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

void main()
{
  vec3 worldPos = mat3(uViewMatrix) * aPosition;
	gl_Position = uProjectionMatrix * vec4(worldPos, 1.0);
	vTexCoord = aPosition;
}

//fragment
#version 330 core

in vec3 vTexCoord;

out vec4 fColor;

uniform samplerCube uTextureSampler;

void main()
{
	fColor = texture(uTextureSampler, vTexCoord);
}
)skyboxshader"
