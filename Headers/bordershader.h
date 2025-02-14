#pragma once
#include "shader.h"
#include "mesh.h"
class BorderShader : public Shader {
	void setUniformIsBorder(bool value) const;
	void setUniformModel(const glm::mat4& model) const;
	void setUniformView(const glm::mat4& view) const;
	void setUniformProjection(const glm::mat4& projection) const;
	void setUniformViewPos(const glm::vec3& viewPos) const;
	void setUniformViewDir(const glm::vec3& viewDir) const;
	void setUniformMaterialShininess(float shininess) const;
	void setUniformMaxPointLights(int count) const;
	void setUniformPointLights(const std::vector<PointLight>& pointLights) const;
	void setUniformDirLight(const DirLight& dirLight) const;
	void setUniformSpotLights(const std::vector<SpotLight>& spotLights) const;
	void setUniformMaxSpotLights(int count) const;

	void renderModel(const Model& model);
public:
	void render(const Scene& scene, const Camera& camera);
	BorderShader(const std::string& vertPath, const std::string& fragPath);
};