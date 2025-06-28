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
#include "../textureutils.h"

LightingShader::LightingShader(std::string_view vertPath, std::string_view fragPath)
	: Shader{ vertPath, fragPath }
{
	use();
	setInt("material.diffuseMap", (int)TextureUtils::Type::diffuse);
	setInt("material.specularMap", (int)TextureUtils::Type::specular);
	setInt("material.emissionMap", (int)TextureUtils::Type::emission);
	setInt("material.reflectionMap", (int)TextureUtils::Type::reflection);
	setInt("skybox", (int)TextureUtils::Type::skybox);
	setInt("shadowMap", 5);
	setFloat("material.shininess", 32);
}

LightingShader::LightingShader(std::string_view vertPath, std::string_view geomPath, std::string_view fragPath)
	: Shader{ vertPath, geomPath, fragPath }
{
	use();
	setInt("material.diffuseMap", (int)TextureUtils::Type::diffuse);
	setInt("material.specularMap", (int)TextureUtils::Type::specular);
	setInt("material.emissionMap", (int)TextureUtils::Type::emission);
	setInt("material.reflectionMap", (int)TextureUtils::Type::reflection);
	setInt("skybox", (int)TextureUtils::Type::skybox);
	setFloat("material.shininess", 32);
}

void LightingShader::setPerFrameUniforms(const Camera& camera, const Scene& scene) const {
	setUniformViewPos(camera.getPos());
	setUniformViewDir(camera.getForward());
	setUniformPointLights(scene.getPointLights());
	setUniformMaxPointLights(scene.getPointLights().size());
	setUniformDirLight(scene.getDirLight());
	setUniformSpotLights(scene.getSpotLights());
	setUniformMaxSpotLights(scene.getSpotLights().size());
	setUniformTime(glfwGetTime());
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(scene.getShadowCaster().getDepthTexture(), GL_TEXTURE_2D);
}

void LightingShader::setUniformDoExploding(bool b) const {
	setBool("doExploding", b);
}
void LightingShader::setUniformTime(float time) const {
	setFloat("time", time);
}
void LightingShader::setUniformModel(const glm::mat4& model) const {
	setMatrix4("model", model);
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
		setVector3(prefix + "pos", pointLights[i].mPosition);
		setVector3(prefix + "color", pointLights[i].mColor);
		setFloat(prefix + "ambientScale", pointLights[i].mAmbientScale);
		setFloat(prefix + "attConst", pointLights[i].mAttenuation.attConst);
		setFloat(prefix + "attLinear", pointLights[i].mAttenuation.attLinear);
		setFloat(prefix + "attQuad", pointLights[i].mAttenuation.attQuad);
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
		setVector3(prefix + "pos", spotLights[i].mPosition);
		setVector3(prefix + "dir", spotLights[i].mDirection);
		setFloat(prefix + "cutoffDot", spotLights[i].mCutoffDot);
		setFloat(prefix + "outerDot", spotLights[i].mOuterDot);

		setVector3(prefix + "color", spotLights[i].mColor);
		setFloat(prefix + "ambientScale", spotLights[i].mAmbientScale);
	}
}
void LightingShader::setUniformMaxSpotLights(int count) const {
	setInt("maxSpotLights", count);
}

void LightingShader::setTexture(unsigned int textureIndex, TextureUtils::Type type) {
	glActiveTexture(GL_TEXTURE0 + (int)type);
	if (type == TextureUtils::Type::skybox) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureIndex);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, textureIndex);
	}
}

void LightingShader::renderModel(const Model& model, const TEX& environmentCubeMapTex) {
	setUniformModel(model.mTransform.getModelMatrix());
	setTexture(environmentCubeMapTex, TextureUtils::Type::skybox);
	for (const Mesh& mesh : model.getMeshes()) {
		setUniformMaterialShininess(mesh.mShininess == 0 ? 1 : mesh.mShininess);
		glBindVertexArray(mesh.mVAO);

		setTexture(mesh.getFirstDiffuseMap(), TextureUtils::Type::diffuse);
		setTexture(mesh.getFirstSpecularMap(), TextureUtils::Type::specular);
		setTexture(mesh.getFirstEmissionMap(), TextureUtils::Type::emission);
		setTexture(mesh.getFirstReflectionMap(), TextureUtils::Type::reflection);

		glDrawElements(GL_TRIANGLES, mesh.mVertexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}