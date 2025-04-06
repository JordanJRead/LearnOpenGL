#version 460 core
out vec4 FragColor;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragWorldPos;

uniform sampler2D diffuseMap;

void main() {
	vec3 lightPos = vec3(0, 5, 0);
	vec3 objectColor = vec3(texture(diffuseMap, fragTexCoord));
	vec3 lightDir = normalize(lightPos - fragWorldPos);
	FragColor = vec4(objectColor * (dot(lightDir, fragNormal) + 1) / 2.0, 1);
}