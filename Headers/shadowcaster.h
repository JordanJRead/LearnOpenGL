#ifndef SHADOW_CASTER_H
#define SHADOW_CASTER_H

#include "shadowframebuffer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class ShadowCaster {
public:
	ShadowCaster(int width, int height, const glm::vec3& position) : mShadowFramebuffer{ width, height } {
		mViewMatrix = glm::lookAt(position, { 0, 0, 0 }, {0, 1, 0});
		mProjectionMatrix = glm::ortho(-100.0, 100.0, -100.0, 100.0, 0.1, 50.0);
	}

	const glm::mat4& getViewMatrix() const {
		return mViewMatrix;
	}

	const glm::mat4& getProjectionMatrix() const {
		return mProjectionMatrix;
	}

	const TEX& getDepthTexture() const {
		return mShadowFramebuffer.getDepthTexture();
	}

	void useFramebuffer() const { mShadowFramebuffer.use(); };

private:
	ShadowFramebuffer mShadowFramebuffer;
	glm::mat4 mViewMatrix;
	glm::mat4 mProjectionMatrix;
};

#endif