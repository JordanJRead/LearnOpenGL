#version 460 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
	vec3 normal;
	vec3 worldPos;
} geom_in[];

layout(std140, binding = 0) uniform Matrices {
	uniform mat4 view;
	uniform mat4 projection;
};

void generateLine(int index) {
	gl_Position = projection * view * vec4(geom_in[index].worldPos, 1);
	EmitVertex();
	
	gl_Position = projection * view * vec4(geom_in[index].worldPos + geom_in[index].normal * 0.3, 1);
	EmitVertex();

	EndPrimitive();
}

void main() {
	generateLine(0);
	generateLine(1);
	generateLine(2);
}