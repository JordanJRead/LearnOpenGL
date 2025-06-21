#pragma once
#include "shader.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "../pointLight.h"
#include "../spotLight.h"

class DirLight;

class BorderShader : public Shader {
public:
	BorderShader(const std::string& vertPath, const std::string& fragPath);
	void setUniformModel(const glm::mat4& model) const; // TODO make renderModel function
};