#version 460 core
out vec4 FragColor;

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

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float attConst;
	float attLinear;
	float attQuad;
};
#define N_POINT_LIGHTS 4
uniform PointLight pointLights[N_POINT_LIGHTS];
uniform int maxPointLights;

struct DirLight {
	vec3 dir;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

struct SpotLight {
	vec3 pos;
	vec3 dir;
	float cutoffDot;
	float outerDot;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
#define N_SPOT_LIGHTS 4
uniform SpotLight spotLights[N_SPOT_LIGHTS];
uniform int maxSpotLights;

uniform samplerCube skybox;

in GEOM_OUT {
	vec3 normal;
	vec3 worldPos;
	vec2 texCoords;
} frag_in;

vec3 CalcDirLight   (DirLight   dirLight,   vec3 normal, vec3 objectColor, vec3 objectSpecularColor);
vec3 CalcPointLight (PointLight pointLight, vec3 normal, vec3 objectColor, vec3 objectSpecularColor);
vec3 CalcSpotLight  (SpotLight  spotLight , vec3 normal, vec3 objectColor, vec3 objectSpecularColor);
void main() {
	vec3 resultColor = vec3(texture(material.emissionMap, frag_in.texCoords));

	vec3 normal = normalize(frag_in.normal);
	vec3 objectColor = vec3(texture(material.diffuseMap, frag_in.texCoords));
	vec3 objectSpecularColor = vec3(texture(material.specularMap, frag_in.texCoords));

	resultColor += CalcDirLight(dirLight, normal, objectColor, objectSpecularColor);
	
	for (int i = 0; i < min(N_POINT_LIGHTS, maxPointLights); i++) {
		resultColor += CalcPointLight(pointLights[i], normal, objectColor, objectSpecularColor);
	}
	for (int i = 0; i < min(N_SPOT_LIGHTS, maxSpotLights); i++) {
		resultColor += CalcSpotLight(spotLights[i], normal, objectColor, objectSpecularColor);
	}

	vec3 incomingRayDir = normalize(frag_in.worldPos - viewPos);

	vec3 reflectDir = reflect(incomingRayDir, normalize(normal));
	vec3 reflectColor = texture(skybox, reflectDir).rgb;
	vec3 reflectMapSample = vec3(texture(material.reflectionMap, frag_in.texCoords));

	//vec3 refractDir = refract(incomingRayDir, normal, 1.0 / 1.52);
	//vec3 refractColor = texture(skybox, refractDir).rgb;

	vec3 finalColor = reflectColor * reflectMapSample + resultColor * (1 - reflectMapSample);

	FragColor = vec4(finalColor, texture(material.diffuseMap, frag_in.texCoords).w);
	//FragColor = vec4(refractColor, texture(material.diffuseMap, fragTexCoords).w);
}

vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 objectColor, vec3 objectSpecularColor) {
	vec3 lightDir = normalize(-dirLight.dir);
	
	vec3 ambientColor = dirLight.ambient * objectColor;

	float diffuseFactor = max(dot(lightDir, normal), 0);
	vec3 diffuseColor = dirLight.diffuse * objectColor * diffuseFactor;

	vec3 reflectDir = reflect(-lightDir, normal);
	float specularFactor = pow(max(dot(-viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularColor = dirLight.specular * objectSpecularColor * specularFactor;
	return diffuseColor + specularColor + ambientColor;
}

vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 objectColor, vec3 objectSpecularColor) {
	vec3 lightDir = normalize(pointLight.pos - frag_in.worldPos);
	
	vec3 ambientColor = pointLight.ambient * objectColor;

	float diffuseFactor = max(dot(lightDir, normal), 0);
	vec3 diffuseColor = pointLight.diffuse * objectColor * diffuseFactor;

	vec3 reflectDir = reflect(-lightDir, normal);
	float specularFactor = pow(max(dot(-viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularColor = pointLight.specular * objectSpecularColor * specularFactor;

	float lightDist = length(pointLight.pos - frag_in.worldPos);
	float attenuation = 1.0 / (pointLight.attConst + pointLight.attLinear * lightDist + pointLight.attQuad * lightDist * lightDist);
	
	return (ambientColor + diffuseColor + specularColor) * attenuation;
}

 // consider adding attenuation to spot lights?
vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 objectColor, vec3 objectSpecularColor) {
	vec3 lightDir = normalize(spotLight.pos - frag_in.worldPos);
	
	vec3 ambientColor = spotLight.ambient * objectColor;

	float diffuseFactor = max(dot(lightDir, normal), 0);
	vec3 diffuseColor = spotLight.diffuse * objectColor * diffuseFactor;

	vec3 reflectDir = reflect(-lightDir, normal);
	float specularFactor = pow(max(dot(-viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularColor = spotLight.specular * objectSpecularColor * specularFactor;

	float intensity = clamp(((dot(lightDir, normalize(-spotLight.dir))) - spotLight.outerDot) / (spotLight.cutoffDot - spotLight.outerDot), 0.0, 1.0); // is not good
	
	return (ambientColor + diffuseColor + specularColor) * intensity;
}