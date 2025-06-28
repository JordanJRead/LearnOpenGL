#ifndef SHADOW_FRAMEBUFFER_H
#define SHADOW_FRAMEBUFFER_H

#include "framebuffer.h"
#include "OpenGL Wrappers/FBO.h"
#include "OpenGL Wrappers/TEX.h"

class ShadowFramebuffer {
public:
	ShadowFramebuffer(int width, int height) {
		glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		glBindTexture(GL_TEXTURE_2D, mDepthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	void use() const { glBindFramebuffer(GL_FRAMEBUFFER, mFBO); }

	const TEX& getDepthTexture() const {
		return mDepthTexture;
	}

private:
	TEX mDepthTexture;
	FBO mFBO;
};

#endif