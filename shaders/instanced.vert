#version 460 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vTexCoord;

out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragWorldPos;

uniform mat4 view;
uniform mat4 projection;
uniform int dim;

vec4 getWorldPos(int id) {

	vec4 worldPos = vec4(0, 0, 0, 1);
	worldPos.x += id % dim - dim / 2.0;
	worldPos.z += id / dim - dim / 2.0;

	return worldPos * 4 + vec4(vPos, 0);
}

void main() {
	vec4 worldPos = getWorldPos(gl_InstanceID);
	gl_Position = projection * view * worldPos;
	fragTexCoord = vTexCoord;
	fragNormal = normalize(vNorm);
	fragWorldPos = worldPos.xyz;
}