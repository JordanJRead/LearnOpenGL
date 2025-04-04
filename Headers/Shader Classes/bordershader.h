#pragma once
#include "shader.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "../pointLight.h"
#include "../spotLight.h"

class DirLight;

class BorderShader : public Shader {
	friend class Renderer;
	void setUniformModel(const glm::mat4& model) const;
	void setUniformView(const glm::mat4& view) const;
	void setUniformProjection(const glm::mat4& projection) const;
public:
	BorderShader(const std::string& vertPath, const std::string& fragPath);
};