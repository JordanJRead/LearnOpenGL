#version 460 core
out vec4 FragColor;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragWorldPos;

uniform sampler2D diffuseMap;

void main() {
	vec3 lightPos = vec3(0, 5, 0);
	vec4 objectColor = texture(diffuseMap, fragTexCoord);
	if (objectColor.a < 0.1) {
		discard;
	}
	//vec3 lightDir = normalize(lightPos - fragWorldPos);
	//FragColor = vec4(vec3(objectColor) * (dot(lightDir, fragNormal) + 1) / 2.0, 1);
	FragColor = objectColor;
}