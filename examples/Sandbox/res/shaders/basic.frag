#version 330 core

layout (location = 0) out vec4 color;

uniform vec2 light_pos;

in DATA
{
	vec2 pos;
	vec2 texCoord;
	float texID;
	vec4 color;
} fs_in;

uniform sampler2D textures[32];

void main()
{
	float intensity = 1.0/pow(length(fs_in.pos - light_pos),0.4f);
	if(intensity > 1)
		intensity = 1;
	//intensity = 1;
	color = fs_in.color;
	if(fs_in.texID>0.0)
	{
		int tid = int(fs_in.texID-0.5);
		color*=texture(textures[tid],fs_in.texCoord);
	}
	color = vec4(color.rgb*intensity,color.a);

	return;

	highp int x = int(gl_FragCoord.x)%2;
	highp int y = int(gl_FragCoord.y)%2;
	if(x==0&&y==0)
		color = vec4(color.r,0,0,color.a);
	else if(y==0)
		color = vec4(0,color.g,0,color.a);
	else if(x==0)
		color = vec4(0,0,color.b,color.a);
	else
		color = color;
		
}