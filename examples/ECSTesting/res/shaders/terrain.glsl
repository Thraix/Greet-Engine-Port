//vertex
#version 330 core

layout(location = 0) in vec3 aPosition;

out vec3 vPos;

void main()
{
  vPos = aPosition;
}

//geometry
#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

uniform vec3 uCameraPos;
uniform mat4 uTransformationMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

uniform vec3 uLightColor = vec3(1.0f, 0.96f, 0.9f);
uniform vec3 uLightPosition = vec3(-100, 100, 0);

in vec3 vPos[];

out vec4 gColor;
out float gVisibility;

const float cDensity = 0.003;
const float cGradient = 1.5;

vec3 GetColorFromHeight(float y)
{
  y /= 20.0f;
  if (y < 0.48)
    return vec3(0.94, 0.89, 0.64);
  else if (y < 0.58)
    return vec3(0.48, 0.83, 0.38);
  else if (y < pow(0.07, 0.6) + 0.58)
    return vec3(0.71, 0.69, 0.65);
  else
    return vec3(0.86, 0.94, 0.94);
}

void main()
{
  vec3 p0 = vec3(uTransformationMatrix * vec4(vPos[0], 1.0f));
  vec3 p1 = vec3(uTransformationMatrix * vec4(vPos[1], 1.0f));
  vec3 p2 = vec3(uTransformationMatrix * vec4(vPos[2], 1.0f));
  vec3 avgPos = (vPos[0] + vPos[1] + vPos[2]) / 3.0f;
  vec3 color = GetColorFromHeight(avgPos.y);
  vec3 normal = normalize(cross(p1 - p0, p2 - p0));

  for(int i=0; i<3; i++)
  {
    gColor = vec4(color, 1.0);
    vec4 worldPosition = uTransformationMatrix * vec4(vPos[i], 1.0f);
    vec4 positionRelativeToCamera = uViewMatrix * worldPosition;
    gl_Position = uProjectionMatrix * positionRelativeToCamera;

    vec3 toCameraVector = uCameraPos - worldPosition.xyz;

    float distance = length(positionRelativeToCamera.xyz);
    gVisibility = exp(-pow((distance*cDensity), cGradient));
    gVisibility = clamp(gVisibility, 0.0, 1.0);
    gColor *= vec4(0.8f, 0.8f, 0.8f, 1.0f);
    vec3 unitLightVector = normalize(uLightPosition);

    float nDot = dot(normal, unitLightVector);
    float brightness = nDot;//max(nDot,0.6);
    if (brightness < 0.6)
      brightness = mix(0.4, 0.6, brightness / 0.6);

    vec3 diffuse = uLightColor * brightness;
    gColor *= vec4(diffuse, 1.0f);
    EmitVertex();
  }
  EndPrimitive();
}

//fragment
#version 330 core

in vec4 gColor;
in float gVisibility;

out vec4 fColor;

uniform vec4 uFogColor = vec4(0.125, 0.125, 0.125, 1);

void main()
{
	fColor = gColor;
	fColor = mix(vec4(uFogColor.rgb, fColor.a), vec4(fColor.rgb, fColor.a), gVisibility);
}
