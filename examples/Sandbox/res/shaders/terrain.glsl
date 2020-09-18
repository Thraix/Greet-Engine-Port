//vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 2) in vec4 aColor;
layout(location = 3) in vec3 aNormal;

flat out vec4 vColor;
flat out float vVisibility;

uniform vec3 uCameraPos;
uniform mat4 uTransformationMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

uniform vec3 uLightColor = vec3(1.0f, 0.96f, 0.9f);
uniform vec3 uLightPosition = vec3(-100, 100, 0);

const float cDensity = 0.003;
const float cGradient = 1.5;

void main()
{
	vColor = vec4(aColor.b, aColor.g, aColor.r, aColor.a);
	vec4 worldPosition = uTransformationMatrix * vec4(aPosition, 1.0f);
	vec4 positionRelativeToCamera = uViewMatrix * worldPosition;
	gl_Position = uProjectionMatrix * positionRelativeToCamera;

	vec3 surfaceNormal = (uTransformationMatrix * vec4(aNormal, 0.0)).xyz;
	vec3 toCameraVector = uCameraPos - worldPosition.xyz;

	float distance = length(positionRelativeToCamera.xyz);
	vVisibility = exp(-pow((distance*cDensity), cGradient));
	vVisibility = clamp(vVisibility, 0.0, 1.0);
	vColor *= vec4(0.8f, 0.8f, 0.8f, 1.0f);
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(uLightPosition);

	float nDot = dot(unitNormal, unitLightVector);
	float brightness = nDot;//max(nDot,0.6);
	if (brightness < 0.6)
		brightness = mix(0.4, 0.6, brightness / 0.6);

	vec3 diffuse = uLightColor * brightness;
	vColor *= vec4(diffuse, 1.0f);
}

//fragment
#version 330 core

flat in vec4 vColor;
flat in float vVisibility;

out vec4 fColor;

uniform vec4 uFogColor = vec4(0.125, 0.125, 0.125, 1);

void main()
{
	fColor = vColor;
	fColor = mix(vec4(uFogColor.rgb, fColor.a), vec4(fColor.rgb, fColor.a), vVisibility);
}
