#version 330 core
layout (location = 0) in vec3 pos;

uniform mat4 model;

//layout(std140, binding = 0) uniform Matrices {
layout(std140) uniform Matrices {
	uniform mat4 view;
	uniform mat4 projection;
};

void main() {
	gl_Position = projection * view * model * vec4(pos, 1);
}