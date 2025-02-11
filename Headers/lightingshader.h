#ifndef LIGHTING_SHADER_H
#define LIGHTING_SHADER_H

#include "shader.h"
class DualShader;

class LightingShader : public Shader {
	friend class DualShader;
protected:
	void setUniformModel(const glm::mat4& model) const;
	void setUniformView(const glm::mat4& model) const;
	void setUniformProjection(const glm::mat4& model) const;

	void setUniformViewPos(const glm::vec3& viewPos) const;
	void setUniformViewDir(const glm::vec3& viewPos) const;

	void setUniformMaterialShininess(float shininess) const;

	void setUniformPointLights(const std::vector<PointLight>& pointLights) const;
	void setUniformMaxPointLights(int count) const;

	void setUniformDirLight(const DirLight& dirLight) const;

	void setUniformSpotLights(const std::vector<SpotLight>& spotLights) const;
	void setUniformMaxSpotLights(int count) const;

	void renderModel(const Model& model) override;
public:
	LightingShader(std::string_view vertPath, std::string_view fragPath);
	void render(const Scene& scene, const Camera& camera) override;
};

#endif