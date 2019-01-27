//vertex
#version 330 core

layout(location = 0) in vec3 position;

out vec4 vert_color;

uniform vec4 mat_color = vec4(1,0,0,1);
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
	vec4 positionRelativeToCamera = viewMatrix * vec4(position, 1.0f);
	gl_Position = projectionMatrix * positionRelativeToCamera;
	vert_color = mat_color;
}

//fragment
#version 330 core

in vec4 vert_color;
out vec4 out_color;

void main()
{
	out_color = vert_color;
	return;
}