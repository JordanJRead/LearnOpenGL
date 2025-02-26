#include "screenquadshader.h"
#include <string>
#include "../scene.h"
#include <chrono>
#include <GLFW/glfw3.h>

ScreenQuadShader::ScreenQuadShader(const std::string& vertPath, const std::string& fragPath): Shader{ vertPath, fragPath } {
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	float vertices[]{
		-1, -1, 0,  0, 0,
		 1, -1, 0,  1, 0,
		-1,  1, 0,  0, 1,

		 1,  1, 0,  1, 1,
		-1,  1, 0,  0, 1,
		 1, -1, 0,  1, 0
	};

	// Top middle small
	for (int i{ 0 }; i < std::size(vertices); ++i) {
		if (i % 5 == 0) { // x pos
			vertices[i] *= 0.2;
		}
		else if (i % 5 == 1) {
			vertices[i] *= 0.2;
			vertices[i] += 0.8;
		}
	}

	glBufferData(GL_ARRAY_BUFFER, std::size(vertices) * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(mID);
	setInt("screenTex", 0);
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

void ScreenQuadShader::render(const Scene& scene, const Camera& camera) {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glUseProgram(mID);

	setFloat("offset", getOffset(mEffectStartTime));

	glBindVertexArray(mVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, scene.mFBOColorTex);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}
void ScreenQuadShader::startEffect() {
	mEffectStartTime = glfwGetTime();
}