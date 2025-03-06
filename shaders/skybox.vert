#version 460 core
layout (location = 0) in vec3 vPos;

out vec3 fragTexDir;

uniform mat4 view;
uniform mat4 projection;

void main() {
	fragTexDir = vPos;
	gl_Position = projection * view * vec4(vPos, 1);
	gl_Position = (projection * view * vec4(vPos, 1)).xyww;
}