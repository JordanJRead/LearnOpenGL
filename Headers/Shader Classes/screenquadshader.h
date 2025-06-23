#pragma once

#include "shader.h"
#include <string>
#include "../OpenGL Wrappers/VAO.h"
#include "../OpenGL Wrappers/BUF.h"

class ScreenQuadShader : public Shader {
	friend class Renderer;
private:
	double mEffectStartTime{ -100 };
	float getOffset(double time);

public:
	ScreenQuadShader(const std::string& vertPath, const std::string& fragPath);
	void setUniformModel(const glm::mat4& model) const;
	void setUniformOffset(float offset) const;
	void startEffect();
};