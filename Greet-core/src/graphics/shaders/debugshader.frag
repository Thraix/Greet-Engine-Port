R"debugshader(
#version 330 core

layout(location = 0) out vec4 color;

in DATA
{
	vec2 pos;
	float texID;
	vec2 texCoord;
	vec4 color;
	float maskTexID;
	vec2 maskTexCoord;
} fs_in;

uniform sampler2D textures[32];

void main()
{
	color = vec4(fs_in.texCoord.xy,0.0,1.0);
}
)debugshader"
