#version 460 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTexCoords;

uniform mat4 model;

out vec2 fragTexCoords;

void main() {
	fragTexCoords = vTexCoords;
	gl_Position = model * vec4(vPos, 1);
	//gl_Position = vec4(vPos, 1);
}