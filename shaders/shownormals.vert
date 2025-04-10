#version 460 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;

out VS_OUT {
	vec3 normal;
	vec3 worldPos;
} vs_out;

uniform mat4 model;

void main() {
	// World space
	vs_out.normal = normalize(mat3(transpose(inverse(model))) * vNorm); // transposing is expensive

	vec4 worldPos4 = model * vec4(vPos, 1);
	vs_out.worldPos = worldPos4.xyz;
	//gl_Position = projection * view * worldPos4;
	gl_Position = worldPos4; // Matrix calculation will be done in the geometry shader
}