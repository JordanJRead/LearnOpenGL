#ifndef LIGHTING_SHADER_H
#define LIGHTING_SHADER_H

#include "shader.h"
#include <vector>
#include "pointLight.h"
/*
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


uniform vec3 viewPos;

struct Material {
	sampler2D diffuseMap;
	sampler2D specularMap;
	sampler2D emissionMap;
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

struct DirLight {
	vec3 dir;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

struct SpotLight {
	vec3 dir;
	float cutoffDot;
	float outerDot;
};
uniform SpotLight spotLight;

*/
class LightingShader : public Shader {
public:
	LightingShader(std::string_view vertPath, std::string_view fragPath)
		: Shader{ vertPath, fragPath }
	{
		setInt("material.diffuseMap", 0);
		setInt("material.specularMap", 1);
		setInt("material.emissionMap", 2);
		setFloat("material.shininess", 32);
	}
	// TODO finish uniforms, update main to use them, finish light classes, test
	void setUniformModel(const glm::mat4& model) const {
		setMatrix4("model", model);
	}
	void setUniformView(const glm::mat4& view) const {
		setMatrix4("view", view);
	}
	void setUniformProjection(const glm::mat4& projection) const {
		setMatrix4("model", projection);
	}
	void setUniformViewPos(const glm::vec3& viewPos) const {
		setVector3("viewPos", viewPos);
	}
	void setUniformMaterialShininess(float shininess) const {
		setFloat("material.shininess", shininess);
	}
	void setUniformPointLights(const std::vector<PointLight>& pointLights) const {
		for (int i{ 0 }; i < 4; ++i) {
			std::string prefix{ "pointLights[" };
			prefix += i;
			prefix += "].";
			setVector3(prefix + "pos", pointLights[i].pos);
			setVector3(prefix + "ambient", pointLights[i].ambient);
			setVector3(prefix + "diffuse", pointLights[i].diffuse);
			setVector3(prefix + "specular", pointLights[i].specular);
		}
	}
};

#endif