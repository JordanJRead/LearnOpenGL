#version 460 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vTexCoords;

out vec3 fragNormal;
out vec3 fragWorldPos;
out vec2 fragTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	fragNormal = normalize(mat3(transpose(inverse(model))) * vNorm); // transposing is expensive

	vec4 worldPos4 = model * vec4(vPos, 1);
	fragWorldPos = worldPos4.xyz;
	fragTexCoords = vTexCoords;
	gl_Position = projection * view * worldPos4;
}