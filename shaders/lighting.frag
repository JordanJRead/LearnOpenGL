#version 460 core
out vec4 FragColor;

in vec3 fragNorm;
in vec3 fragWorldPos;
in vec2 texCoords;

uniform vec3 lightColor;
uniform vec3 viewPos;

struct Material {
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D emissionMap;
	float shininess;
};
uniform Material material;

struct Light {
	vec3 pos;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;

void main() {
	vec3 norm = normalize(fragNorm);
	vec3 color = vec3(texture(material.diffuseMap, texCoords));

	vec3 ambient = light.ambient * color;

	vec3 lightDir = normalize(light.pos - fragWorldPos);
	float diffuseFactor = max(dot(lightDir, norm), 0);
	vec3 diffuse = light.diffuse * color * diffuseFactor;

	vec3 viewDir = normalize(viewPos - fragWorldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float specFactor = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * vec3(texture(material.specularMap, texCoords)) * specFactor;

	vec3 result = ambient + diffuse + specular + vec3(texture(material.emissionMap, texCoords));
	FragColor = vec4(result, 1);
}