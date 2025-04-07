#version 460 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vTexCoords;

out VS_OUT {
	vec3 normal;
	vec3 worldPos;
	vec2 texCoords;
} vs_out;

uniform mat4 model;

layout(std140, binding = 0) uniform Matrices {
	uniform mat4 view;
	uniform mat4 projection;
};

void main() {
	vs_out.normal = normalize(mat3(transpose(inverse(model))) * vNorm); // transposing is expensive

	vec4 worldPos4 = model * vec4(vPos, 1);
	vs_out.worldPos = worldPos4.xyz;
	vs_out.texCoords = vTexCoords;
	gl_Position = projection * view * worldPos4;
}