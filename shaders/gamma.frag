#version 460 core

in vec2 fragTexCoord;
uniform sampler2D image;

out vec4 FragColor;

void main() {
	float gamma = 2.2;
	FragColor = texture(image, fragTexCoord);
	FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}