#pragma once

#include "shader.h"
#include <string>
#include "../OpenGL Wrappers/VAO.h"
#include "../OpenGL Wrappers/VBO.h"

class ScreenQuadShader : public Shader {
	friend class Renderer;
private:
	double mEffectStartTime{ -100 };
	float getOffset(double time);

public:
	ScreenQuadShader(const std::string& vertPath, const std::string& fragPath);
	void setUniformModel(const glm::mat4& model);
	void setUniformOffset(float offset);
	void startEffect();
};