//vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 color;
layout(location = 3) in vec3 normal;

out vec4 vert_color;
out vec2 vert_texCoord;
out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCameraVector;
out float visibility;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 light_position = vec3(0.0, 0.0, -10.0);

const float density = 0.007;
const float gradient = 1.5;

void main()
{

	vec4 worldPosition = transformationMatrix * vec4(position, 1.0f);
	vec4 positionRelativeToCamera = viewMatrix * worldPosition;
	gl_Position = projectionMatrix * positionRelativeToCamera;
	vert_color = vec4(color.b, color.g, color.r, color.a);

	vert_texCoord = texCoord;
	surfaceNormal = (transformationMatrix * vec4(normal, 0.0)).xyz;
	toLightVector = light_position - worldPosition.xyz;
	toCameraVector = (inverse(viewMatrix) * vec4(0, 0, 0, 1)).xyz - worldPosition.xyz;

	float distance = length(positionRelativeToCamera.xyz);
	visibility = exp(-pow((distance*density), gradient));
	visibility = clamp(visibility, 0.0, 1.0);
}

//fragment
#version 330 core

in vec4 vert_color;
in vec2 vert_texCoord;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;
in float visibility;

layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_brightColor;

uniform sampler2D textureSampler;
uniform vec3 light_color = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 fogColor;
uniform float hasTexture = 1.0;
uniform float shadeDamper = 10.0;
uniform float reflectivity = 1;

void main()
{
	out_color = vert_color;
	out_color *= vec4(0.6f, 0.6f, 0.6f, 1.0f);
	if (hasTexture > 0.5)
	{
		out_color *= texture(textureSampler, vert_texCoord);
		if (out_color.a < 0.1)
			discard;
	}
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
	out_color *= vec4(diffuse, 1.0f);
	out_color += vec4(finalSpecular, 0.0);

	out_color = mix(vec4(fogColor.xyz, 1.0), vec4(out_color.rgb, 1.0), visibility);
	float luminance = (out_color.r + out_color.g + out_color.b) / 3.0;
	//out_color.rgb = floor(luminance * 4.0)/4.0 * fogColor;
	if (luminance > 0.8)
		out_brightColor = vec4(luminance, luminance, luminance, 1);
	else
		out_brightColor = vec4(0, 0, 0, 1);
	//out_color = vec4(finalSpecular.xyz,1.0);
	//out_color = vec4(out_color.rgb*visibility,1.0);
	//out_color.a = 1.0f;
	//out_color = vec4(surfaceNormal*0.5f+0.5f,1.0f);

}