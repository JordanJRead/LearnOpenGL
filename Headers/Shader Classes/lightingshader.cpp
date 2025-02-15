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

LightingShader::LightingShader(std::string_view vertPath, std::string_view fragPath)
	: Shader{ vertPath, fragPath }
{
	use();
	setInt("material.diffuseMap", 0);
	setInt("material.specularMap", 1);
	setInt("material.emissionMap", 2);
	setFloat("material.shininess", 32);
}

void LightingShader::renderModel(const Model& model) {
	setUniformModel(model.mModel);
	for (const Mesh& mesh : model.getMeshes()) {
		setUniformMaterialShininess(mesh.mShininess);
		glBindVertexArray(mesh.mVAO);

		glActiveTexture(GL_TEXTURE0);
		int diffuseMap{ mesh.getFirstDiffuse() };
		if (diffuseMap >= 0) {
			glBindTexture(GL_TEXTURE_2D, diffuseMap);
		}

		glActiveTexture(GL_TEXTURE1);
		int specularMap{ mesh.getFirstSpecular() };
		if (specularMap >= 0) {
			glBindTexture(GL_TEXTURE_2D, specularMap);
		}

		glDrawElements(GL_TRIANGLES, mesh.mVertexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void LightingShader::render(const Scene& scene, const Camera& camera) {
	use();
	setUniformView(camera.mView);
	setUniformProjection(camera.mProjection); // is once per frame best?
	setUniformViewPos(camera.mPos);
	setUniformViewDir(camera.mForward);

	setUniformPointLights(scene.getPointLights());
	setUniformMaxPointLights(scene.getPointLights().size());
	setUniformDirLight(scene.getDirLight());
	setUniformSpotLights(scene.getSpotLights());
	setUniformMaxSpotLights(scene.getSpotLights().size());
	for (const Model& model : scene.getModels()) {
		renderModel(model);
	}

	std::vector<float> vertices{
		-1, -1, 0, 0, 0, -1, 0, 0,
		-1, 1, 0, 0, 0, -1, 0, 1,
		1, -1, 0, 0, 0, -1, 1, 0,

		1, 1, 0, 0, 0, -1, 1, 1,
		-1, 1, 0, 0, 0, -1, 0, 1,
		1, -1, 0, 0, 0, -1, 1, 0
	};

	unsigned int grassVAO;
	unsigned int grassVBO;
	glGenVertexArrays(1, &grassVAO);
	glGenBuffers(1, &grassVBO);

	glBindVertexArray(grassVAO);
	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
	setUniformMaterialShininess(64);
	for (const glm::vec3& grassPos : scene.getGrassPositions()) {
		glm::mat4 model{ 1 };
		model = glm::translate(model, grassPos);
		setUniformModel(model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, scene.grassDiffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, scene.grassSpecular);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &grassVAO);
	glDeleteBuffers(1, &grassVBO);
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