#version 460 core
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in vec3 gColor[];
out vec3 fColor;

void main() {
	fColor = gColor[0];
	gl_Position = gl_in[0].gl_Position + vec4(-0.1, -0.1, 0, 0);
	EmitVertex();
	
	gl_Position = gl_in[0].gl_Position + vec4(0.1, -0.1, 0, 0);
	EmitVertex();
	
	gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.1, 0, 0);
	EmitVertex();
	
	gl_Position = gl_in[0].gl_Position + vec4(0.1, 0.1, 0, 0);
	EmitVertex();
	
	fColor = vec3(1, 1, 1);
	gl_Position = gl_in[0].gl_Position + vec4(0, 0.2, 0, 0);
	EmitVertex();

	EndPrimitive();
}