#version 330 core

const float gaussFilter[7] = { 0.015625f,0.09375f,0.234375f,0.3125f,0.234375f,0.09375f,0.015625f}


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

out vec4 out_color;

uniform sampler2D texture;
uniform vec2 scale;

void main()
{
	out_color = 0.0;
	for(int = -3;i<=3;i++)
	{
		out_color += texture2D(texture,vec2(fs_in.texCoord.x+i*scale.x, fs_in.texCoord+i*scale.y))*gaussFilter[i+3];
	}
}