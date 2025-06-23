#include "screenquadshader.h"
#include <string>
#include "../scene.h"
#include <chrono>
#include <GLFW/glfw3.h>

ScreenQuadShader::ScreenQuadShader(const std::string& vertPath, const std::string& fragPath): Shader{ vertPath, fragPath } {
	glUseProgram(mID);
	setInt("screenTex", 0);
}

void ScreenQuadShader::setUniformModel(const glm::mat4& model) const {
	setMatrix4("model", model);
}
void ScreenQuadShader::setUniformOffset(float offset) const {
	setFloat("offset", offset);
}

double easeOutBounce(double x) {
	const double n1 = 7.5625;
	const double d1 = 2.75;

	if (x < 1 / d1) {
		return n1 * x * x;
	}
	 else if (x < 2 / d1) {
		return n1 * (x -= 1.5 / d1) * x + 0.75;
	}
	else if (x < 2.5 / d1) {
		return n1 * (x -= 2.25 / d1) * x + 0.9375;
	}
	else {
		return n1 * (x -= 2.625 / d1) * x + 0.984375;
	}
}

double easeInOutBounce(double x) {
	return x < 0.5
	  ? (1 - easeOutBounce(1 - 2 * x)) / 2
	  : (1 + easeOutBounce(2 * x - 1)) / 2;
}

float ScreenQuadShader::getOffset(double time) {
	constexpr double effectStartValue{ 0.1 };
	constexpr double effectTime{ 2 };
	double timeElapsed{ glfwGetTime() - mEffectStartTime };
	// 0 -> effectStartValue
	// effectTime -> 0
	double easingInput{ timeElapsed / effectTime };
	easingInput = (easeInOutBounce(easingInput));
	if (timeElapsed > effectTime) return 0;
	return effectStartValue - (effectStartValue * easingInput);
}

void ScreenQuadShader::startEffect() {
	mEffectStartTime = glfwGetTime();
}