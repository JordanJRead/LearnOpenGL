#version 460 core
out vec4 FragColor;

uniform vec3 viewPos;

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

in GEOM_OUT {
	vec3 normal;
	vec3 worldPos;
	vec3 shadowNDCPos;
	vec2 texCoords;
} frag_in;

uniform sampler2D shadowMap;
uniform bool depth;
vec3 CalcDirLight   (DirLight   dirLight,   vec3 normal, vec3 objectColor, vec3 objectSpecularColor, vec3 viewDir);
vec3 CalcPointLight (PointLight pointLight, vec3 normal, vec3 objectColor, vec3 objectSpecularColor, vec3 viewDir);
vec3 CalcSpotLight  (SpotLight  spotLight , vec3 normal, vec3 objectColor, vec3 objectSpecularColor, vec3 viewDir);
void main() {
	vec3 resultColor = vec3(texture(material.emissionMap, frag_in.texCoords));

	vec3 normal = normalize(frag_in.normal);
	vec3 objectColor = vec3(texture(material.diffuseMap, frag_in.texCoords));
	vec3 objectSpecularColor = vec3(texture(material.specularMap, frag_in.texCoords));

	vec3 viewDir = normalize(viewPos - frag_in.worldPos);

	resultColor += CalcDirLight(dirLight, normal, objectColor, objectSpecularColor, viewDir	);
	
	for (int i = 0; i < min(N_POINT_LIGHTS, maxPointLights); i++) {
		resultColor += CalcPointLight(pointLights[i], normal, objectColor, objectSpecularColor, viewDir);
	}
	for (int i = 0; i < min(N_SPOT_LIGHTS, maxSpotLights); i++) {
		resultColor += CalcSpotLight(spotLights[i], normal, objectColor, objectSpecularColor, viewDir);
	}

	vec3 incomingRayDir = normalize(frag_in.worldPos - viewPos);

	vec3 reflectDir = reflect(incomingRayDir, normalize(normal));
	vec3 reflectColor = texture(skybox, reflectDir).rgb;
	vec3 reflectMapSample = vec3(texture(material.reflectionMap, frag_in.texCoords));

	//vec3 refractDir = refract(incomingRayDir, normal, 1.0 / 1.52);
	//vec3 refractColor = texture(skybox, refractDir).rgb;

	vec3 finalColor = reflectColor * reflectMapSample + resultColor * (1 - reflectMapSample);

	FragColor = vec4(finalColor, texture(material.diffuseMap, frag_in.texCoords).w);
}

vec3 CalcDirLight(DirLight dirLight, vec3 normal, vec3 objectColor, vec3 objectSpecularColor, vec3 viewDir) {
	vec3 lightDir = normalize(-dirLight.dir);
	
	vec3 ambientColor = dirLight.color * dirLight.ambientScale * objectColor;

	float diffuseFactor = max(dot(lightDir, normal), 0);
	vec3 diffuseColor = dirLight.color * objectColor * diffuseFactor;

	vec3 halfwayVector = normalize(viewDir + lightDir);
	float specularFactor = pow(max(dot(normal, halfwayVector), 0.0), material.shininess);
	vec3 specularColor = dirLight.color * objectSpecularColor * specularFactor;
	
	vec2 shadowMapCoordinate = (frag_in.shadowNDCPos.xy + vec2(1, 1)) * 0.5;
	float closestShadowDepth = texture(shadowMap, shadowMapCoordinate).r;
	float currentShadowDepth = (frag_in.shadowNDCPos.z + 1) / 2.0;
	float bias = max(0.05 * (1.0 - dot(normal, normalize(vec3(0.1, 10, 0.3)))), 0.005) * 0.3;

	if (closestShadowDepth + bias < currentShadowDepth) {
		return ambientColor;
	}
	return diffuseColor + specularColor + ambientColor;
}

vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 objectColor, vec3 objectSpecularColor, vec3 viewDir) {
	vec3 lightDir = normalize(pointLight.pos - frag_in.worldPos);
	
	vec3 ambientColor = pointLight.color * pointLight.ambientScale * objectColor;

	float diffuseFactor = max(dot(lightDir, normal), 0);
	vec3 diffuseColor = pointLight.color * objectColor * diffuseFactor;
	
	vec3 halfwayVector = normalize(viewDir + lightDir);
	float specularFactor = pow(max(dot(normal, halfwayVector), 0.0), material.shininess);
	vec3 specularColor = pointLight.color * objectSpecularColor * specularFactor;

	float lightDist = length(pointLight.pos - frag_in.worldPos);
	float attenuation = 1.0 / (pointLight.attConst + pointLight.attLinear * lightDist + pointLight.attQuad * lightDist * lightDist);
	
	return (ambientColor + diffuseColor + specularColor) * attenuation;
}

 // consider adding attenuation to spot lights?
vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 objectColor, vec3 objectSpecularColor, vec3 viewDir) {
	vec3 lightDir = normalize(spotLight.pos - frag_in.worldPos);
	
	vec3 ambientColor = spotLight.color * spotLight.ambientScale * objectColor;

	float diffuseFactor = max(dot(lightDir, normal), 0);
	vec3 diffuseColor = spotLight.color * objectColor * diffuseFactor;
	
	vec3 halfwayVector = normalize(viewDir + lightDir);
	float specularFactor = pow(max(dot(normal, halfwayVector), 0.0), material.shininess);
	vec3 specularColor = spotLight.color * objectSpecularColor * specularFactor;

	float intensity = clamp(((dot(lightDir, normalize(-spotLight.dir))) - spotLight.outerDot) / (spotLight.cutoffDot - spotLight.outerDot), 0.0, 1.0); // is not good
	
	return (ambientColor + diffuseColor + specularColor) * intensity;
}