R"skyboxshader(
#version 330 core

in vec3 vert_texCoord;
out vec4 out_color;
uniform samplerCube textureSampler;

void main()
{
	out_color = texture(textureSampler,vert_texCoord);
}
)skyboxshader"
