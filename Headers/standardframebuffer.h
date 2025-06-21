#ifndef STANDARD_FRAMEBUFFER_H
#define STANDARD_FRAMEBUFFER_H

#include <glad/glad.h>
#include "OpenGL Wrappers/FBO.h"
#include "OpenGL Wrappers/TEX.h"
#include "OpenGL Wrappers/RBO.h"
#include <vector>

// A framebuffer that has a color texture and depth/stencil rbo
class StandardFramebuffer {

public:
	StandardFramebuffer(int width, int height);
	std::vector<GLubyte> getImageData() const;
	int getImageWidth() const;
	int getImageHeight() const;
	void use() { glBindFramebuffer(GL_FRAMEBUFFER, mFBO); }

private:
	FBO mFBO;
	TEX mColorTexture;
	RBO mDepthStencilRBO;
	int mWidth;
	int mHeight;
};

#endif