#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 normalVector;

vec3 getNormal(vec3 a, vec3 b, vec3 c) {
	vec3 ab = b - a;
	vec3 bc = c - b;
	return normalize(cross(bc, ab));
}

void main() {
	normalVector = getNormal(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz);
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();
	EndPrimitive();
}