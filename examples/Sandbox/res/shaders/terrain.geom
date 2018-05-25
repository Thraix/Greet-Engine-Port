#version 330

out vec4 vert_color;
out vec3 surfaceNormal;
out vec3 toLightVector;
out float visibility;


const float density = 0.005;
const float gradient = 1.3;

in Vertex
{
	vec3 realPos;
	vec4 realColor;
} vertex[];

uniform mat4 transformationMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 light_position = vec3(0.0,1000.0,0.0);
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

void main()
{
	vec3 v1 = vertex[1].realPos.xyz - vertex[0].realPos.xyz;
	vec3 v2 = vertex[2].realPos.xyz - vertex[0].realPos.xyz;
	vec3 normal = cross(v1,v2);
	float m = max(max(vertex[0].realPos.y,vertex[1].realPos.y),vertex[2].realPos.y);
	if(m == vertex[0].realPos.y)
		vert_color = vertex[0].realColor;
	else if(m == vertex[1].realPos.y)
		vert_color = vertex[1].realColor;
	else
		vert_color = vertex[2].realColor;

	surfaceNormal = (transformationMatrix * vec4(normal,0.0)).xyz;
	toLightVector = vec3(0,light_position.y - (transformationMatrix * (vec4( (vertex[0].realPos+vertex[1].realPos+vertex[2].realPos) / 3.0,1.0) )).y,1.0);

	for(int i = 0;i<3;i++)
	{
		gl_Position = gl_in[i].gl_Position;

		vec4 worldPosition = transformationMatrix * vec4(vertex[i].realPos,1.0f);
		vec4 positionRelativeToCamera = viewMatrix * worldPosition;

		float distance = length(positionRelativeToCamera.xyz);
		visibility = exp(-pow((distance*density),gradient));
		visibility = clamp(visibility,0.0,1.0);
		EmitVertex();
	}
	EndPrimitive();
}
