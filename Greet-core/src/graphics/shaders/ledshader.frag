R"ledshader(
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
	color = fs_in.color;
	if (fs_in.texID>0.0)
	{
		int tid = int(fs_in.texID - 0.5);
		color *= texture(textures[tid], fs_in.texCoord);
	}
highp int x = int(gl_FragCoord.x) % 2;
highp int y = int(gl_FragCoord.y) % 2;
if (x == 0 && y == 0)
	color = vec4(color.r, 0, 0, color.a);
else if (y == 0)
	color = vec4(0, color.g, 0, color.a);
else if (x == 0)
	color = vec4(0, 0, color.b, color.a);
}
)ledshader"