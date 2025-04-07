#pragma once

#include "shader.h"
#include "../OpenGL Wrappers/VAO.h"
#include "../OpenGL Wrappers/BUF.h"

class SkyBoxShader : public Shader {
	friend class Renderer;
private:
	void setUniformSkyBoxTexture(int unit) const;

public:
	SkyBoxShader(std::string_view vertPath, std::string_view fragPath);
};