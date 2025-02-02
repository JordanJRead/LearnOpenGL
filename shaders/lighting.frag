#version 460 core
out vec4 FragColor;

in vec3 fragNorm;
in vec3 fragWorldPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;

void main() {
	vec3 norm = normalize(fragNorm);

	float ambientFactor = 0.1;
	vec3 ambient = lightColor * ambientFactor;

	vec3 lightDir = normalize(lightPos - fragWorldPos);
	float diffuseFactor = max(dot(lightDir, norm), 0);
	vec3 diffuse = diffuseFactor * lightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - fragWorldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float specFactor = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specular = specularStrength * specFactor * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1);
}