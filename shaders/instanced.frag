#version 460 core
out vec4 FragColor;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragWorldPos;

uniform sampler2D diffuseMap;
uniform sampler2D perlinNoise;
uniform sampler2D yellowTex;

in vec2 noiseCoords;

void main() {
	vec3 lightPos = vec3(0, 5, 0);
	vec4 objectColor = texture(diffuseMap, fragTexCoord);
	if (objectColor.a < 0.1) {
		discard;
	}
	//vec3 lightDir = normalize(lightPos - fragWorldPos);
	//FragColor = vec4(vec3(objectColor) * (dot(lightDir, fragNormal) + 1) / 2.0, 1);
	vec4 goldColor = vec4(203/255.0, 206/255.0, 70/255.0, 1);
	FragColor = objectColor + texture(perlinNoise, noiseCoords) * 1.0 * (texture(yellowTex, fragTexCoord) - objectColor);
}