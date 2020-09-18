R"(
#version 330 core

layout(location = 0) in vec3 aPosition;

uniform mat4 uProjectionMatrix = mat4(1.0);
uniform mat4 uViewMatrix = mat4(1.0);
uniform mat4 uTransformationMatrix = mat4(1.0);

void main()
{
  gl_Position = uProjectionMatrix * uViewMatrix * uTransformationMatrix * vec4(aPosition, 1.0);
}
)"
