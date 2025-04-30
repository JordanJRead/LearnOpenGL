#include <vector>
#include "../pointLight.h"
#include "../structs.h"
#include "../spotLight.h"
#include "../camera.h"
#include "../scene.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "gouraudshader.h"
#include <glad/glad.h>
#include "shader.h"
#include <iostream>
#include "../model.h"
#include "../mesh.h"
#include "../OpenGL Wrappers/VAO.h"
#include "../OpenGL Wrappers/BUF.h"
#include "../textureutils.h"

GouraudShader::GouraudShader(std::string_view vertPath, std::string_view fragPath)
	: Shader{ vertPath, fragPath }
{
	use();
	setInt("material.diffuseMap", (int)TextureUtils::Type::diffuse);
	setInt("material.specularMap", (int)TextureUtils::Type::specular);
	setInt("material.emissionMap", (int)TextureUtils::Type::emission);
	setInt("material.reflectionMap", (int)TextureUtils::Type::reflection);
	setInt("skybox", (int)TextureUtils::Type::skybox);
	setFloat("material.shininess", 32);

	unsigned int lights_index = glGetUniformBlockIndex(mID, "Matrices"); //todo chance var name
	glUniformBlockBinding(mID, lights_index, 0);

}

void GouraudShader::setPerFrameUniforms(const Camera& camera, const Scene& scene) const {
	setUniformViewPos(camera.getPos()); //todo viewspace calculations
	setUniformViewDir(camera.getForward());
	setUniformPointLights(scene.getPointLights());
	setUniformMaxPointLights(scene.getPointLights().size());
	setUniformDirLight(scene.getDirLight());
	setUniformSpotLights(scene.getSpotLights());
	setUniformMaxSpotLights(scene.getSpotLights().size());
}

void GouraudShader::setUniformModel(const glm::mat4& model) const {
	setMatrix4("model", model);
}
void GouraudShader::setUniformViewPos(const glm::vec3& viewPos) const {
	setVector3("viewPos", viewPos);
}
void GouraudShader::setUniformViewDir(const glm::vec3& viewDir) const {
	setVector3("viewDir", viewDir);
}
void GouraudShader::setUniformMaterialShininess(float shininess) const {
	setFloat("material.shininess", shininess);
}
void GouraudShader::setUniformMaxPointLights(int count) const {
	setInt("maxPointLights", count);
}
void GouraudShader::setUniformPointLights(const std::vector<PointLight>& pointLights) const {
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
void GouraudShader::setUniformDirLight(const DirLight& dirLight) const {
	setVector3("dirLight.dir", dirLight.dir);
	setVector3("dirLight.ambient", dirLight.colors.ambient);
	setVector3("dirLight.diffuse", dirLight.colors.diffuse);
	setVector3("dirLight.specular", dirLight.colors.specular);
}
void GouraudShader::setUniformSpotLights(const std::vector<SpotLight>& spotLights) const {
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
void GouraudShader::setUniformMaxSpotLights(int count) const {
	setInt("maxSpotLights", count);
}

void GouraudShader::setTexture(unsigned int textureIndex, TextureUtils::Type type) {
	glActiveTexture(GL_TEXTURE0 + (int)type);
	if (type == TextureUtils::Type::skybox) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureIndex);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, textureIndex);
	}
}

void GouraudShader::renderModel(const Model& model, const TEX& environmentCubeMapTex, const TextureUtils::DefaultTextures2D& defaultTextures) {
	setUniformModel(model.mModel);
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