R"basiclightshader(
#version 330 core

layout(location = 0) out vec4 color;
uniform vec2 light_pos;

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
float intensity = 1.0/pow(length(fs_in.pos - light_pos),0.4f);
if (intensity > 1)
	intensity = 1;
	color = fs_in.color;
	if (fs_in.texID>0.0)
	{
		int tid = int(fs_in.texID - 0.5);
		color *= texture(textures[tid], fs_in.texCoord);
	}
color = vec4(color.rgb*intensity, color.a);
}
)basiclightshader"