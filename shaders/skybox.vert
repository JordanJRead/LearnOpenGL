#version 460 core
layout (location = 0) in vec3 vPos;

out vec3 fragTexDir;

layout(std140, binding = 0) uniform Matrices {
	uniform mat4 view;
	uniform mat4 projection;
};

void main() {
	fragTexDir = vPos;
	gl_Position = (projection * mat4(mat3(view)) * vec4(vPos, 1)).xyww;
}