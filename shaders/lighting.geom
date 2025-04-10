#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
	vec3 normal;
	vec3 worldPos;
	vec2 texCoords;
} geom_in[];

out GEOM_OUT {
	vec3 normal;
	vec3 worldPos;
	vec2 texCoords;
} geom_out;


layout(std140, binding = 0) uniform Matrices {
	uniform mat4 view;
	uniform mat4 projection;
};

uniform float time;
uniform bool doExploding;

vec4 explode(vec4 pos, vec3 normal) {
	return pos + vec4(normal * (sin(time) + 1), 0);
}

vec3 getNormal() {
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(b, a));
}

void main() {
	if (doExploding) {
		vec3 normal = getNormal();

		gl_Position = projection * view * explode(gl_in[0].gl_Position, normal);
		geom_out.normal = geom_in[0].normal;
		geom_out.worldPos = geom_in[0].worldPos;
		geom_out.texCoords = geom_in[0].texCoords;
		EmitVertex();
	
		gl_Position = projection * view * explode(gl_in[1].gl_Position, normal);
		geom_out.normal = geom_in[1].normal;
		geom_out.worldPos = geom_in[1].worldPos;
		geom_out.texCoords = geom_in[1].texCoords;
		EmitVertex();
	
		gl_Position = projection * view * explode(gl_in[2].gl_Position, normal);
		geom_out.normal = geom_in[2].normal;
		geom_out.worldPos = geom_in[2].worldPos;
		geom_out.texCoords = geom_in[2].texCoords;
		EmitVertex();

		EndPrimitive();
	}
	else {
		gl_Position = projection * view * gl_in[0].gl_Position;
		geom_out.normal = geom_in[0].normal;
		geom_out.worldPos = geom_in[0].worldPos;
		geom_out.texCoords = geom_in[0].texCoords;
		EmitVertex();
	
		gl_Position = projection * view * gl_in[1].gl_Position;
		geom_out.normal = geom_in[1].normal;
		geom_out.worldPos = geom_in[1].worldPos;
		geom_out.texCoords = geom_in[1].texCoords;
		EmitVertex();
	
		gl_Position = projection * view * gl_in[2].gl_Position;
		geom_out.normal = geom_in[2].normal;
		geom_out.worldPos = geom_in[2].worldPos;
		geom_out.texCoords = geom_in[2].texCoords;
		EmitVertex();

		EndPrimitive();
	}
}