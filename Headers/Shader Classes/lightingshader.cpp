#include <vector>
#include "../pointLight.h"
#include "../structs.h"
#include "../spotLight.h"
#include "../camera.h"
#include "../scene.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "lightingshader.h"
#include <glad/glad.h>
#include "shader.h"
#include <iostream>
#include "../model.h"
#include "../mesh.h"
#include "../OpenGL Wrappers/VAO.h"
#include "../OpenGL Wrappers/BUF.h"

LightingShader::LightingShader(std::string_view vertPath, std::string_view fragPath)
	: Shader{ vertPath, fragPath }
{
	use();
	setInt("material.diffuseMap", mDiffuseTextureIndex);
	setInt("material.specularMap", mSpecularTextureIndex);
	setInt("material.emissionMap", mEmissionTextureIndex);
	setInt("material.reflectionMap", mReflectionTextureIndex);
	setInt("skybox", mSkyboxTextureIndex);
	setFloat("material.shininess", 32);
}

void LightingShader::setUniformModel(const glm::mat4& model) const {
	setMatrix4("model", model);
}
void LightingShader::setUniformView(const glm::mat4& view) const {
	setMatrix4("view", view);
}
void LightingShader::setUniformProjection(const glm::mat4& projection) const {
	setMatrix4("projection", projection);
}
void LightingShader::setUniformViewPos(const glm::vec3& viewPos) const {
	setVector3("viewPos", viewPos);
}
void LightingShader::setUniformViewDir(const glm::vec3& viewDir) const {
	setVector3("viewDir", viewDir);
}
void LightingShader::setUniformMaterialShininess(float shininess) const {
	setFloat("material.shininess", shininess);
}
void LightingShader::setUniformMaxPointLights(int count) const {
	setInt("maxPointLights", count);
}
void LightingShader::setUniformPointLights(const std::vector<PointLight>& pointLights) const {
	std::size_t iMax{ pointLights.size() < 4 ? pointLights.size() : 4 };
	for (std::size_t i{ 0 }; i < iMax; ++i) {
		std::string prefix{ "pointLights[" };
		prefix += std::to_string(i);
		prefix += "].";
		setVector3(prefix + "pos", pointLights[i].transform.pos);
		setVector3(prefix + "ambient", pointLights[i].colors.ambient);
		setVector3(prefix + "diffuse", pointLights[i].colors.diffuse);
		setVector3(prefix + "specular", pointLights[i].colors.specular);
		setFloat(prefix + "attConst", pointLights[i].attenuation.attConst);
		setFloat(prefix + "attLinear", pointLights[i].attenuation.attLinear);
		setFloat(prefix + "attQuad", pointLights[i].attenuation.attQuad);
	}
}
void LightingShader::setUniformDirLight(const DirLight& dirLight) const {
	setVector3("dirLight.dir", dirLight.dir);
	setVector3("dirLight.ambient", dirLight.colors.ambient);
	setVector3("dirLight.diffuse", dirLight.colors.diffuse);
	setVector3("dirLight.specular", dirLight.colors.specular);
}
void LightingShader::setUniformSpotLights(const std::vector<SpotLight>& spotLights) const {
	std::size_t iMax{ spotLights.size() < 4 ? spotLights.size() : 4 };
	for (std::size_t i{ 0 }; i < iMax; ++i) {
		std::string prefix{ "spotLights[" };
		prefix += std::to_string(i);
		prefix += "].";
		setVector3(prefix + "pos", spotLights[i].transform.pos);
		setVector3(prefix + "dir", spotLights[i].dir);
		setFloat(prefix + "cutoffDot", spotLights[i].cutoffDot);
		setFloat(prefix + "outerDot", spotLights[i].outerDot);

		setVector3(prefix + "ambient", spotLights[i].colors.ambient);
		setVector3(prefix + "diffuse", spotLights[i].colors.diffuse);
		setVector3(prefix + "specular", spotLights[i].colors.specular);
	}
}
void LightingShader::setUniformMaxSpotLights(int count) const {
	setInt("maxSpotLights", count);
}