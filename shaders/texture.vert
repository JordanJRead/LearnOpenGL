#version 460 core
layout (location=0) in vec2 vPos;
layout (location=1) in vec2 vTexCoords;

out vec2 fragTexCoords;

void main() {
	fragTexCoords = vTexCoords;
	gl_Position = vec4(vPos.xy, 0, 1);
}