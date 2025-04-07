#version 460 core
layout (location = 0) in vec3 vPos;

uniform mat4 model;

layout(std140, binding = 0) uniform Matrices {
	uniform mat4 view;
	uniform mat4 projection;
};

void main() {
	gl_Position = projection * view * model * vec4(vPos * 1.1, 1);
}