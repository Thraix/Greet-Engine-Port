//vertex 
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 3) in vec3 normal;

out vec4 vert_color;
out vec2 vert_texCoord;

uniform vec4 mat_color;
uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
	vert_texCoord = texCoord;
	vec4 worldPosition = transformationMatrix * vec4(position, 1.0f);
	vec4 positionRelativeToCamera = viewMatrix * worldPosition;
	gl_Position = projectionMatrix * positionRelativeToCamera;

	vert_color = mat_color;
  return;

	// X major
	if (abs(normal.x) > abs(normal.y) && abs(normal.x) > abs(normal.z))
		vert_color *= vec4(1.0f, 1.0f, 1.0f, 1.0f);
	// Y major
	else if (abs(normal.y) > abs(normal.z))
		vert_color *= vec4(0.9f, 0.9f, 0.9f, 1.0f);
	// Z major
	else
		vert_color *= vec4(0.8f, 0.8f, 0.8f, 1.0f);
}

//fragment
#version 330 core

in vec4 vert_color;
in vec2 vert_texCoord;
out vec4 out_color;

void main()
{
	float xAbs = abs(vert_texCoord.x - 0.5f);
	float yAbs = abs(vert_texCoord.y - 0.5f);
	float value = xAbs > yAbs ? xAbs : yAbs;
	value = pow(1.0 - value*2,0.1);
	//out_color = vert_color * texture(tex, vert_texCoord);
	out_color = vert_color * vec4(value, value, value, 1);
	//out_color = vec4(vert_texCoord, 1, 1);
}
