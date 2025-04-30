#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vTexCoord;

out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragWorldPos;

//layout(std140, binding = 0) uniform Matrices {
layout(std140) uniform Matrices {
	uniform mat4 view;
	uniform mat4 projection;
};

uniform int dim;
uniform float time;

out vec2 noiseCoords;
uniform sampler2D perlinNoise;

float rand(uint x)
{
    x ^= x >> 16;
    x *= 0x7feb352dU;
    x ^= x >> 15;
    x *= 0x846ca68bU;
    x ^= x >> 16;
    return x / 4294967296.0;
}

vec4 getWorldPos(int id) {
	float random = rand(uint(id));
	float density = 0.2f;

	vec4 worldPoint = vec4(0, 0, 0, 1);
	worldPoint.x += density * (id % dim - dim / 2.0) + 0.5 * random;
	worldPoint.z += density * (id / dim - dim / 2.0) + 0.5 * random;

	mat4 rot;
	float theta = random * 2 * 3.14159;
	float cT = cos(theta);
	float sT = sin(theta);

	rot[0] = vec4(cT, 0, sT, 0);
	rot[1] = vec4(0, 1, 0, 0);
	rot[2] = vec4(-sT, 0, cT, 0);
	rot[3] = vec4(0, 0, 0, 1);
	
	vec4 worldPos = vec4(vPos, 1);
	worldPos.y += 1;

	if (vPos.y > 0.5) {
		worldPos.y += 3 * random; // Height varience
		worldPos = rot * worldPos;
		worldPos.x += 0.8 * sin(0.5 * time);
		return worldPoint * 4 + worldPos;
	}
	else {
		return worldPoint * 4 + rot * worldPos;
	}

}

void main() {
	vec4 worldPos = getWorldPos(gl_InstanceID);
	gl_Position = projection * view * worldPos;
	fragTexCoord = vTexCoord;
	fragNormal = normalize(vNorm);
	fragWorldPos = worldPos.xyz;
	noiseCoords = worldPos.xz / 300.0; // bigger means more spread out
}