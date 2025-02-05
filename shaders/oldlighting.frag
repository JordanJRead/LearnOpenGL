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
	float attLinear;
	float attQuad;
};
uniform Light light;

uniform vec3 flashLightDir;
uniform float cutoffDot;
uniform float outerDot;

void main() {
	vec3 color = vec3(texture(material.diffuseMap, texCoords));
	vec3 ambient = light.ambient * color;
	float lightDist = length(light.pos - fragWorldPos);
	float att = 1.0 / (1 + lightDist * light.attLinear + lightDist * lightDist * light.attQuad);

	vec3 viewDir = normalize(viewPos - fragWorldPos);
	float flashLightDot = dot(normalize(flashLightDir), normalize(-viewDir));
	float intensity = (flashLightDot - outerDot) / (cutoffDot - outerDot);
	intensity = max(intensity, 0.1);
	intensity = min(intensity, 1);

	vec3 norm = normalize(fragNorm);

	vec3 lightDir = normalize(light.pos - fragWorldPos);
	float diffuseFactor = max(dot(lightDir, norm), 0);
	vec3 diffuse = light.diffuse * color * diffuseFactor;

	vec3 reflectDir = reflect(-lightDir, norm);
	float specFactor = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * vec3(texture(material.specularMap, texCoords)) * specFactor;

	vec3 result = ambient + diffuse + specular;
	result *= att;
	FragColor = vec4(result, 1) * intensity;
}