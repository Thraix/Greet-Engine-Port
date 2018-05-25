#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float texID;
layout(location = 3) in vec4 color;
layout(location = 4) in vec2 maskTexCoord;
layout(location = 5) in float maskTexID;

uniform mat3 pr_matrix = mat3(1.0);
uniform mat3 ml_matrix = mat3(1.0);

out DATA
{
	vec2 pos;
	float texID;
	vec2 texCoord;
	vec4 color;
	float maskTexID;
	vec2 maskTexCoord;
} vs_out;

void main()
{
	gl_Position = vec4(pr_matrix * ml_matrix * vec3(position, 1.0), 1.0);
	vs_out.pos = vec2(ml_matrix * vec3(position, 1.0));
	vs_out.texCoord = vec2(texCoord.x, 1 - texCoord.y);
	vs_out.texID = texID;
	vs_out.color = vec4(color.z, color.y, color.x, color.w);
	vs_out.maskTexCoord = vec2(maskTexCoord.x, 1 - maskTexCoord.y);
	vs_out.maskTexID =  maskTexID;
}
