//vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec4 aColor;
layout(location = 3) in vec3 aNormal;

flat out vec4 vColor;
flat out vec2 vTexCoord;
flat out vec3 vSurfaceNormal;
flat out vec3 vToLightVector;
flat out vec3 vToCameraVector;
out float vVisibility;

uniform mat4 uTransformationMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform vec3 uLightPosition = vec3(0.0, 0.0, -10.0);

const float cDensity = 0.007;
const float cGradient = 1.5;

void main()
{

	vec4 worldPosition = uTransformationMatrix * vec4(aPosition, 1.0f);
	vec4 aPositionRelativeToCamera = uViewMatrix * worldPosition;
	gl_Position = uProjectionMatrix * aPositionRelativeToCamera;
	vColor = vec4(aColor.b, aColor.g, aColor.r, aColor.a);

	vTexCoord = aTexCoord;
	vSurfaceNormal = (uTransformationMatrix * vec4(aNormal, 0.0)).xyz;
	vToLightVector = uLightPosition - worldPosition.xyz;
	vToCameraVector = (inverse(uViewMatrix) * vec4(0, 0, 0, 1)).xyz - worldPosition.xyz;

	float distance = length(aPositionRelativeToCamera.xyz);
	vVisibility = exp(-pow((distance*cDensity), cGradient));
	vVisibility = clamp(vVisibility, 0.0, 1.0);
}

//fragment
#version 330 core

flat in vec2 vTexCoord;
flat in vec4 vColor;
flat in vec3 vSurfaceNormal;
flat in vec3 vToLightVector;
flat in vec3 vToCameraVector;
in float vVisibility;

out vec4 fColor;

uniform sampler2D uTexture;
uniform vec3 uLightColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 uFogColor;
uniform float uHasTexture = 1.0;
uniform float uShadeDamper = 10.0;
uniform float uReflectivity = 1;

void main()
{
	fColor = vColor;
	fColor *= vec4(0.6f, 0.6f, 0.6f, 1.0f);
	if (uHasTexture > 0.5)
	{
		fColor *= texture(uTexture, vTexCoord);
		if (fColor.a < 0.1)
			discard;
	}
	vec3 unitNormal = normalize(vSurfaceNormal);
	vec3 unitLightVector = normalize(vToLightVector);

	float nDot = dot(unitNormal, unitLightVector);
	float brightness = max(nDot, 0.5);
	vec3 diffuse = uLightColor * brightness;

	vec3 unitVectorToCamera = normalize(vToCameraVector);
	vec3 lightDirection = -unitLightVector;
	vec3 reflection = reflect(lightDirection, unitNormal);

	float specularFactor = dot(reflection, unitVectorToCamera);
	specularFactor = max(specularFactor, 0.0);

	float dampedFactor = pow(specularFactor, uShadeDamper);

	vec3 finalSpecular = dampedFactor  * uReflectivity * uLightColor;
	fColor *= vec4(diffuse, 1.0f);
	fColor += vec4(finalSpecular, 0.0);

	fColor = mix(vec4(uFogColor.xyz, 1.0), vec4(fColor.rgb, 1.0), vVisibility);
}
