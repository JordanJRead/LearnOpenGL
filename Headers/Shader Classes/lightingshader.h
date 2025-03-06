#ifndef LIGHTING_SHADER_H
#define LIGHTING_SHADER_H

#include "shader.h"
#include "glm/glm.hpp"
#include "string_view"
#include "string"
#include <vector>
#include "../pointLight.h"
#include "../spotLight.h"

class DirLight;

class LightingShader : public Shader {
	friend class Renderer;
protected:
	int mDiffuseTextureIndex{ 0 };
	int mSpecularTextureIndex{ 1 };
	int mEmissionTextureIndex{ 2 };
	int mReflectionTextureIndex{ 3 };
	int mSkyboxTextureIndex{ 4 };

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
public:
	LightingShader(std::string_view vertPath, std::string_view fragPath);
};

#endif