R"default2dshader(
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
		//color = vec4(fs_in.texCoord.xy,0.0,1.0);
	}
	if (fs_in.maskTexID>0.0)
	{
		int mtid = int(fs_in.maskTexID - 0.5);
		color.rgb *= texture(textures[mtid],fs_in.maskTexCoord).r;
	}
}
)default2dshader"