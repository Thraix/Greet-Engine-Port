//vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 4) in vec4 offsets;

flat out vec4 vert_color;
flat out float visibility;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

uniform vec3 light_color = vec3(1.0f, 0.96f, 0.9f);
uniform float time;
uniform float waterLevel = 0.45 * 100;
uniform vec4 fogColor = vec4(0.125, 0.125, 0.125, 1);
uniform float shadeDamper = 10.0;
uniform float reflectivity = 1;

const float density = 0.003;
const float gradient = 1.5;
const float waveLength = 4.0;
const float waveAmplitude = 0.2;
const float PI = 3.1415926535897932384626433832795;

float generateOffset(float x, float z, float val1, float val2){
	float radiansX = ((mod(x+z*x*val1, waveLength)/waveLength) + time * 0.125 * mod(x * 0.8 + z, 1.5)) * 2.0 * PI;
	float radiansZ = ((mod(val2 * (z*x +x*z), waveLength)/waveLength) + time * 0.25 * mod(x , 2.0) ) * 2.0 * PI;
	return waveAmplitude * 0.5 * (sin(radiansZ) + cos(radiansX));
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
	vert_color = vec4(0.5, 0.9, 1, 0.5);
  vec3 pos = GetPosition(vec3(position.x, waterLevel, position.z));
  vec3 pos1 = GetPosition(vec3(position.x + offsets.x, waterLevel, position.z + offsets.y)); 
  vec3 pos2 = GetPosition(vec3(position.x + offsets.z, waterLevel, position.z + offsets.w)); 
  vec3 normal = normalize(cross(pos1 - pos, pos2 - pos));
	vec4 worldPosition = transformationMatrix * vec4(pos,1.0);
	vec4 positionRelativeToCamera = viewMatrix * worldPosition;
	gl_Position = projectionMatrix * positionRelativeToCamera;

	vec3 surfaceNormal = (transformationMatrix * vec4(normal, 0.0)).xyz;
	vec3 toLightVector = vec3(-100, 100, 0);
	vec3 toCameraVector = (inverse(viewMatrix) * vec4(0, 0, 0, 1)).xyz - worldPosition.xyz;

	float distance = length(positionRelativeToCamera.xyz);
	visibility = exp(-pow((distance*density), gradient));
	visibility = clamp(visibility, 0.0, 1.0);
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(toLightVector);

	float nDot = dot(unitNormal, unitLightVector);
	float brightness = max(nDot, 0.5);
	vec3 diffuse = light_color * brightness;

	vec3 unitVectorToCamera = normalize(toCameraVector);
	vec3 lightDirection = -unitLightVector;
	vec3 reflection = reflect(lightDirection, unitNormal);

	float specularFactor = dot(reflection, unitVectorToCamera);
	specularFactor = max(specularFactor, 0.0);

	float dampedFactor = pow(specularFactor, shadeDamper);

	vec3 finalSpecular = dampedFactor  * reflectivity * light_color;
	vert_color *= vec4(diffuse, 1.0f);
	vert_color += vec4(finalSpecular, 0.0);

	vert_color = mix(vec4(fogColor.xyz, vert_color.a), vec4(vert_color), visibility);
	vert_color *= vec4(diffuse, 1.0f);

  float fresnel = ((1-dot(unitNormal, unitVectorToCamera)) + 0.5) * 0.5;
  vert_color.a = fresnel;
}

//fragment
#version 330 core

flat in vec4 vert_color;
flat in float visibility;


out vec4 out_color;

void main()
{
	out_color = vert_color;
}
