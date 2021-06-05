#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

void main() {
	gl_Position = gl_in[0].gl_Position + vec4(0.05, 0.0, 0.0, 0.0);
	EmitVertex();
	gl_Position = gl_in[1].gl_Position + vec4(0.0, 0.05, 0.0, 0.0);
	EmitVertex();
	gl_Position = gl_in[2].gl_Position + vec4(0.0, 0.0, 0.05, 0.0);
	EmitVertex();
	EndPrimitive();
}