#pragma once

#include "shader.h"
#include "../OpenGL Wrappers/VAO.h"
#include "../OpenGL Wrappers/VBO.h"

class SkyBoxShader : public Shader {
	friend class Renderer;
private:
	void setUniformView(const glm::mat4& view) const;
	void setUniformProjection(const glm::mat4& projection) const;
	void setUniformSkyBoxTexture(int unit) const;

public:
	SkyBoxShader(std::string_view vertPath, std::string_view fragPath);
};