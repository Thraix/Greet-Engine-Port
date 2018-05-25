#version 330

const float gaussFilter[7] = float[]( 0.015625f,0.09375f,0.234375f,0.3125f,0.234375f,0.09375f,0.015625f);


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
	out_color = vec4(0,0,0,0);
	for(int i = -3;i<=3;i++)
	{
		out_color += texture2D(texture,(fs_in.texCoord+i*scale))*gaussFilter[i+3];
	}
}