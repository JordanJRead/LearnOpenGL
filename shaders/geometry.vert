#version 460 core

layout (location=0) in vec2 vPos;
layout (location=1) in vec3 vColor;

out vec3 gColor;

void main() {
	gl_Position = vec4(vPos.x, vPos.y, 0, 1);
	gColor = vColor;
}