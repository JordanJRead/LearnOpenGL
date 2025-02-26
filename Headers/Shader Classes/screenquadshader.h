#pragma once

#include "shader.h"
#include <string>
#include "../OpenGL Wrappers/VAO.h"
#include "../OpenGL Wrappers/VBO.h"

class ScreenQuadShader : public Shader {
private:
	VAO mVAO;
	VBO mVBO;
	double mEffectStartTime{ -100 };
	float getOffset(double time);

public:
	ScreenQuadShader(const std::string& vertPath, const std::string& fragPath);
	void render(const Scene& scene, const Camera& camera) override;
	void startEffect();
};