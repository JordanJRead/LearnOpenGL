#version 460 core
out vec4 FragColor;

uniform samplerCube cubeMap;

in vec3 fragTexDir;

void main() {
	FragColor = texture(cubeMap, fragTexDir);
}