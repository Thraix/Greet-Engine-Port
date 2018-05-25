//vertex
#version 330 core

layout(location = 0) in vec3 position;

out vec3 vert_texCoord;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
	gl_Position = (projectionMatrix * viewMatrix *  vec4(position, 1.0f));
	vert_texCoord = position;
}

//fragment
#version 330 core

in vec3 vert_texCoord;

out vec4 out_color;

uniform samplerCube textureSampler;

void main()
{
	out_color = texture(textureSampler, vert_texCoord);
}