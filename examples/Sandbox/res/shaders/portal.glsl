//vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 3) in vec3 normal;

out vec4 vert_color;
out vec2 vert_texCoord;
out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCameraVector;
out float visibility;

uniform vec3 uCameraPos;
uniform mat4 uTransformationMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform vec3 uLightPosition = vec3(0.0, 0.0, -10.0);
uniform vec4 uMaterialColor = vec4(1,1,1,1);

const float density = 0.007;
const float gradient = 1.5;

void main()
{

	vec4 worldPosition = uTransformationMatrix * vec4(aPosition, 1.0f);
	vec4 positionRelativeToCamera = uViewMatrix * worldPosition;
	gl_Position = uProjectionMatrix * positionRelativeToCamera;
	vert_color = uMaterialColor * vec4(1,1,1,1);

	vert_texCoord = aPosition.xz;
	surfaceNormal = (uTransformationMatrix * vec4(normal, 0.0)).xyz;
	toLightVector = uLightPosition - worldPosition.xyz;
	toCameraVector = uCameraPos - worldPosition.xyz;

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
uniform float specularExponent = 10.0;
uniform float specularStrength = 1;
uniform float diffuseStrength = 1;
uniform float ambient = 0.3;
uniform float time;

void main()
{
	out_color = vert_color;
	if (hasTexture > 0.5)
	{
    vec2 pos = vert_texCoord;
    float r = sqrt(dot(pos,pos));
    float a = atan(pos.y, pos.x) * 0.5 / 3.1415;

		vec4 texColor = texture(textureSampler, vec2(r - a + time/10.0,a + time/2.0));
    float maxR = sqrt(2);

    r /= maxR;

    out_color *= vec4(pow(r,1) * texColor.rgb, texColor.a);
		if (out_color.a < 0.1)
			discard;
    return;
	}
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(toLightVector);
	vec3 unitVectorToCamera = normalize(toCameraVector);
	vec3 lightDirection = -unitLightVector;

	float diffuse = diffuseStrength * max(dot(unitNormal, unitLightVector), 0.0);
	float specular = specularStrength * pow(max(dot(reflect(lightDirection, unitNormal), unitVectorToCamera), 0.0f), specularExponent);

	out_color *= vec4((ambient + (diffuse + specular) * light_color), 1.0f);
	out_color = mix(vec4(fogColor.xyz, 1.0), vec4(out_color.rgb, 1.0), visibility);
}
