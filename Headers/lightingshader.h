#ifndef LIGHTING_SHADER_H
#define LIGHTING_SHADER_H

#include "shader.h"
#include <vector>
#include "pointLight.h"
#include "structs.h"
#include "spotLight.h"
#include "camera.h"
#include "scene.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class LightingShader : public Shader {
private:
	void setUniformView(const glm::mat4& model) const;
	void setUniformProjection(const glm::mat4& model) const;

	void setUniformViewPos(const glm::vec3& viewPos) const;
	void setUniformViewDir(const glm::vec3& viewPos) const;
	void setUniformMaterialShininess(float shininess) const;
	void setUniformPointLights(const std::vector<PointLight>& pointLights) const;
	void setUniformDirLight(const DirLight& dirLight);
	void setUniformMaxPointLights(int count) const;
	void setUniformSpotLights(const std::vector<SpotLight>& spotLights) const;
	void setUniformMaxSpotLights(int count) const;

public:
	void setUniformModel(const glm::mat4& model) const;
	LightingShader(std::string_view vertPath, std::string_view fragPath)
		: Shader{ vertPath, fragPath }
	{
		use();
		setInt("material.texture_diffuse0", 0);
		setInt("material.texture_diffuse1", 1);
		setInt("material.emissionMap", 2);
		setFloat("material.shininess", 32);
	}

	void render(const Scene& scene, const Camera& camera) override {
		use();
		setUniformView(camera.view);
		setUniformProjection(camera.projection); // is once per frame best?
		setUniformViewPos(camera.pos);
		setUniformViewDir(camera.forward);

		setUniformPointLights(scene.getPointLights());
		setUniformMaxPointLights(scene.getPointLights().size());
		setUniformDirLight(scene.getDirLight());
		setUniformSpotLights(scene.getSpotLights());
		setUniformMaxSpotLights(scene.getSpotLights().size());

		for (const Object& object : scene.getObjects()) {
			setUniformModel(object.modelInfo.model);
			setUniformMaterialShininess(object.shininess);
			object.use();
			glDrawArrays(GL_TRIANGLES, 0, object.modelInfo.vertexCount);
		}
	//	std::vector<float> vertices{
	//		-0.5, -0.5, 0,
	//		0.5, -0.5, 0,
	//		0, 0.5, 0
	//	};
	//	unsigned int VAO;
	//	glGenVertexArrays(1, &VAO);
	//	glBindVertexArray(VAO);

	//	unsigned int VBO;
	//	glGenBuffers(1, &VBO);
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);


	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//	glEnableVertexAttribArray(0);

	//	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//	//glEnableVertexAttribArray(1);

	//	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//	//glEnableVertexAttribArray(2);

	//	glm::mat4 model{ 1 };
	//	setUniformModel(model);
	//	setUniformProjection(model); // is once per frame best?
	//	setUniformView(model);
	//	setUniformMaterialShininess(32.0f);

	//	glDrawArrays(GL_TRIANGLES, 0, 36);

	//	glBindVertexArray(0);
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	glDeleteVertexArrays(1, &VAO);
	//	glDeleteBuffers(1, &VBO);

	}
};

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
		setVector3(prefix + "pos",       pointLights[i].transform.pos);
		setVector3(prefix + "ambient",   pointLights[i].colors.ambient);
		setVector3(prefix + "diffuse",   pointLights[i].colors.diffuse);
		setVector3(prefix + "specular",  pointLights[i].colors.specular);
		setFloat(  prefix + "attConst",  pointLights[i].attenuation.attConst);
		setFloat(  prefix + "attLinear", pointLights[i].attenuation.attLinear);
		setFloat(  prefix + "attQuad",   pointLights[i].attenuation.attQuad);
	}
}
void LightingShader::setUniformDirLight(const DirLight& dirLight) {
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
#endif