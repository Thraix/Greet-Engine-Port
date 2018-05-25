#version 330

out vec4 vert_color;

layout(points) in;
layout(triangle_strip, max_vertices = 3) out;


void main()
{
	vec2 center = gl_in[0].gl_Position.xy;
	gl_Position = vec4(center+vec2(0,-0.5f),0,1);
	vert_color = vec4(0.0,1.0,1.0,1.0);
	EmitVertex();
	gl_Position = vec4(center+vec2(-0.5f,0.5f),0,1);
	vert_color = vec4(0.0,0.0,1.0,1.0);
	EmitVertex();
	gl_Position = vec4(center+vec2(0.5f,0.5f),0,1);
	vert_color = vec4(1.0,1.0,0.0,1.0);
	EmitVertex();
	EndPrimitive();
}