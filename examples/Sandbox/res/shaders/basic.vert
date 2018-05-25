#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in float texID;
layout (location = 3) in vec4 color;

uniform mat3 pr_matrix;
uniform mat3 ml_matrix = mat3(1.0);

out DATA
{
	vec2 pos;
	vec2 texCoord;
	float texID;
	vec4 color;
} vs_out;

void main()
{
	gl_Position = vec4(pr_matrix * ml_matrix * vec3(position,1.0),1.0);
	vs_out.pos = vec2(ml_matrix * vec3(position,1.0));
	vs_out.texCoord = vec2(texCoord.x,1-texCoord.y);
	vs_out.texID = texID;
	vs_out.color = vec4(color.z,color.y,color.x,color.w);
}