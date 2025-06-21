#version 460 core
layout (location = 0) in vec3 pos;

uniform vec3 position;

layout(std140, binding = 0) uniform Matrices {
	uniform mat4 view;
	uniform mat4 projection;
};

void main() {
	gl_Position = projection * view * vec4(pos * 0.5 + position, 1);
}