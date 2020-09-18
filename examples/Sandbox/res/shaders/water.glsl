//vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 4) in vec4 offsets;

flat out vec4 vColor;
flat out float vVisibility;

uniform vec3 uCameraPos;
uniform mat4 uTransformationMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

uniform vec3 uLightColor = vec3(1.0f, 0.96f, 0.9f);
uniform float uTime;
uniform float uWaterLevel = 0.45 * 100;
uniform float uShadeDamper = 10.0;
uniform float uReflectivity = 1;

uniform vec4 uFogColor = vec4(0.125, 0.125, 0.125, 1);
uniform float cFogDensity = 0.003;
uniform float cFogGradient = 1.5;

const float cWaveLength = 4.0;
const float cWaveAmplitude = 0.2;
const float cPI = 3.1415926535897932384626433832795;

float generateOffset(float x, float z, float val1, float val2){
	float radiansX = ((mod(x+z*x*val1, cWaveLength)/cWaveLength) + uTime * 0.125 * mod(x * 0.8 + z, 1.5)) * 2.0 * cPI;
	float radiansZ = ((mod(val2 * (z*x +x*z), cWaveLength)/cWaveLength) + uTime * 0.25 * mod(x , 2.0) ) * 2.0 * cPI;
	return cWaveAmplitude * 0.5 * (sin(radiansZ) + cos(radiansX));
}

vec3 GetPosition(vec3 pos)
{
	float xDistortion = generateOffset(pos.x, pos.z, 0.2, 0.1);
	float yDistortion = generateOffset(pos.x, pos.z, 0.1, 0.3);
	float zDistortion = generateOffset(pos.x, pos.z, 0.15, 0.2);
	return pos + vec3(xDistortion, yDistortion, zDistortion);
}

void main()
{
	vColor = vec4(0.5, 0.9, 1, 0.5);
  vec3 pos = GetPosition(vec3(position.x, uWaterLevel, position.z));
  vec3 pos1 = GetPosition(vec3(position.x + offsets.x, uWaterLevel, position.z + offsets.y));
  vec3 pos2 = GetPosition(vec3(position.x + offsets.z, uWaterLevel, position.z + offsets.w));
  vec3 normal = normalize(cross(pos1 - pos, pos2 - pos));
	vec4 worldPosition = uTransformationMatrix * vec4(pos,1.0);
	vec4 positionRelativeToCamera = uViewMatrix * worldPosition;
	gl_Position = uProjectionMatrix * positionRelativeToCamera;

	vec3 surfaceNormal = (uTransformationMatrix * vec4(normal, 0.0)).xyz;
	vec3 toLightVector = vec3(-100, 100, 0);
	vec3 toCameraVector = (inverse(uViewMatrix) * vec4(0, 0, 0, 1)).xyz - worldPosition.xyz;

	float distance = length(positionRelativeToCamera.xyz);
	vVisibility = exp(-pow((distance*cFogDensity), cFogGradient));
	vVisibility = clamp(vVisibility, 0.0, 1.0);
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(toLightVector);

	float nDot = dot(unitNormal, unitLightVector);
	float brightness = max(nDot, 0.5);
	vec3 diffuse = uLightColor * brightness;

	vec3 unitVectorToCamera = normalize(toCameraVector);
	vec3 lightDirection = -unitLightVector;
	vec3 reflection = reflect(lightDirection, unitNormal);

	float specularFactor = dot(reflection, unitVectorToCamera);
	specularFactor = max(specularFactor, 0.0);

	float dampedFactor = pow(specularFactor, uShadeDamper);

	vec3 finalSpecular = dampedFactor  * uReflectivity * uLightColor;
	vColor *= vec4(diffuse, 1.0f);
	vColor += vec4(finalSpecular, 0.0);

	vColor = mix(vec4(uFogColor.xyz, vColor.a), vec4(vColor), vVisibility);
	vColor *= vec4(diffuse, 1.0f);

  float fresnel = ((1-dot(unitNormal, unitVectorToCamera)) + 0.5) * 0.5;
  vColor.a = fresnel;
}

//fragment
#version 330 core

flat in vec4 vColor;
flat in float vVisibility;


out vec4 fColor;

void main()
{
	fColor = vColor;
}
