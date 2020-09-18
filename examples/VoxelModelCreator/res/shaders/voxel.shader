//vertex 
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 3) in vec3 aNormal;

out vec4 vColor;
out vec2 vertTexCoord;

uniform mat4 uTransformationMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

uniform vec4 uMaterialColor;

void main()
{
	vertTexCoord = aTexCoord;
	vec4 worldPosition = uTransformationMatrix * vec4(aPosition, 1.0f);
	vec4 positionRelativeToCamera = uViewMatrix * worldPosition;
	gl_Position = uProjectionMatrix * positionRelativeToCamera;

	vColor = uMaterialColor;
  return;

	// X major
	if (abs(aNormal.x) > abs(aNormal.y) && abs(aNormal.x) > abs(aNormal.z))
		vColor *= vec4(1.0f, 1.0f, 1.0f, 1.0f);
	// Y major
	else if (abs(aNormal.y) > abs(aNormal.z))
		vColor *= vec4(0.9f, 0.9f, 0.9f, 1.0f);
	// Z major
	else
		vColor *= vec4(0.8f, 0.8f, 0.8f, 1.0f);
}

//fragment
#version 330 core

in vec4 vColor;
in vec2 vertTexCoord;

out vec4 fColor;

void main()
{
	float xAbs = abs(vertTexCoord.x - 0.5f);
	float yAbs = abs(vertTexCoord.y - 0.5f);
	float value = xAbs > yAbs ? xAbs : yAbs;
	value = pow(1.0 - value*2,0.1);
	//fColor = vColor * texture(tex, vertTexCoord);
	fColor = vColor * vec4(value, value, value, 1);
	//fColor = vec4(vertTexCoord, 1, 1);
}
