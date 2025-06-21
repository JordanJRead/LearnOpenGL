#version 460 core

// Vertex information
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vTexCoord;

uniform mat4 model;

layout(std140, binding = 0) uniform Matrices {
	uniform mat4 view;
	uniform mat4 projection;
};

// Lighting information
out vec4 fragColor;

uniform vec3 viewPos;
uniform vec3 viewDir;

struct Material {
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D emissionMap;
	sampler2D reflectionMap;
	float shininess;
};
uniform Material material;

struct PointLight {
	vec3 pos;

	vec3 color;
	float ambientScale;

	float attConst;
	float attLinear;
	float attQuad;
};
#define N_POINT_LIGHTS 4
uniform PointLight pointLights[N_POINT_LIGHTS];
uniform int maxPointLights;

struct DirLight {
	vec3 dir;

	vec3 color;
	float ambientScale;
};
uniform DirLight dirLight;

struct SpotLight {
	vec3 pos;
	vec3 dir;
	float cutoffDot;
	float outerDot;
	
	vec3 color;
	float ambientScale;
};
#define N_SPOT_LIGHTS 4
uniform SpotLight spotLights[N_SPOT_LIGHTS];
uniform int maxSpotLights;

uniform samplerCube skybox;

vec3 CalcDirLight   (DirLight   dirLight,   vec3 normal, vec3 objectColor, vec3 objectSpecularColor);
vec3 CalcPointLight (PointLight pointLight, vec3 normal, vec3 objectColor, vec3 objectSpecularColor, vec3 worldPos);
vec3 CalcSpotLight  (SpotLight  spotLight , vec3 normal, vec3 objectColor, vec3 objectSpecularColor, vec3 worldPos);

void main() {
	// Vertex information
	vec3 normal = normalize(mat3(transpose(inverse(model))) * vNorm); // transposing is expensive

	vec4 worldPos4 = model * vec4(vPos, 1);
	vec3 worldPos = worldPos4.xyz;
	gl_Position = projection * view * worldPos4;

	vec3 resultColor = vec3(texture(material.emissionMap, vTexCoord));

	vec3 objectColor = vec3(texture(material.diffuseMap, vTexCoord));
	vec3 objectSpecularColor = vec3(texture(material.specularMap, vTexCoord));

	//resultColor += CalcDirLight(dirLight, normal, objectColor, objectSpecularColor);
	
	for (int i = 0; i < min(N_POINT_LIGHTS, maxPointLights); i++) {
		resultColor += CalcPointLight(pointLights[i], normal, objectColor, objectSpecularColor, worldPos);
	}
	for (int i = 0; i < min(N_SPOT_LIGHTS, maxSpotLights); i++) {
		resultColor += CalcSpotLight(spotLights[i], normal, objectColor, objectSpecularColor, worldPos);
	}

	vec3 incomingRayDir = normalize(worldPos - viewPos);

	vec3 reflectDir = reflect(incomingRayDir, normalize(normal));
	vec3 reflectColor = texture(skybox, reflectDir).rgb;
	vec3 reflectMapSample = vec3(texture(material.reflectionMap, vTexCoord));

	//vec3 refractDir = refract(incomingRayDir, normal, 1.0 / 1.52);
	//vec3 refractColor = texture(skybox, refractDir).rgb;

	vec3 finalColor = reflectColor * reflectMapSample + resultColor * (1 - reflectMapSample);

	fragColor = vec4(finalColor, texture(material.diffuseMap, vTexCoord).w);
	//FragColor = vec4(refractColor, texture(material.diffuseMap, fragTexCoords).w);
}

vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 objectColor, vec3 objectSpecularColor) {
	vec3 lightDir = normalize(-dirLight.dir);
	
	vec3 ambientColor = dirLight.color * dirLight.ambientScale * objectColor;

	float diffuseFactor = max(dot(lightDir, normal), 0);
	vec3 diffuseColor = dirLight.color * objectColor * diffuseFactor;

	vec3 reflectDir = reflect(-lightDir, normal);
	float specularFactor = pow(max(dot(-viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularColor = dirLight.color * objectSpecularColor * specularFactor;
	return diffuseColor + specularColor + ambientColor;
}

vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 objectColor, vec3 objectSpecularColor, vec3 worldPos) {
	vec3 lightDir = normalize(pointLight.pos - worldPos);
	
	vec3 ambientColor = pointLight.color * pointLight.ambientScale * objectColor;

	float diffuseFactor = max(dot(lightDir, normal), 0);
	vec3 diffuseColor = pointLight.color * objectColor * diffuseFactor;

	vec3 reflectDir = reflect(-lightDir, normal);
	float specularFactor = pow(max(dot(-viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularColor = pointLight.color * objectSpecularColor * specularFactor;

	float lightDist = length(pointLight.pos - worldPos);
	float attenuation = 1.0 / (pointLight.attConst + pointLight.attLinear * lightDist + pointLight.attQuad * lightDist * lightDist);
	
	return (ambientColor + diffuseColor + specularColor) * attenuation;
}

 // consider adding attenuation to spot lights?
vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 objectColor, vec3 objectSpecularColor, vec3 worldPos) {
	vec3 lightDir = normalize(spotLight.pos - worldPos);
	
	vec3 ambientColor = spotLight.color * spotLight.ambientScale * objectColor;

	float diffuseFactor = max(dot(lightDir, normal), 0);
	vec3 diffuseColor = spotLight.color * objectColor * diffuseFactor;

	vec3 reflectDir = reflect(-lightDir, normal);
	float specularFactor = pow(max(dot(-viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularColor = spotLight.color * objectSpecularColor * specularFactor;

	float intensity = clamp(((dot(lightDir, normalize(-spotLight.dir))) - spotLight.outerDot) / (spotLight.cutoffDot - spotLight.outerDot), 0.0, 1.0); // is not good
	
	return (ambientColor + diffuseColor + specularColor) * intensity;
}