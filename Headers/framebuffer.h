#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>
#include "OpenGL Wrappers/FBO.h"
#include "OpenGL Wrappers/TEX.h"
#include "OpenGL Wrappers/RBO.h"
#include <vector>

class Framebuffer {
public:
	Framebuffer(int width, int height) : mWidth{ width }, mHeight{ height } {}
	virtual std::vector<GLubyte> getImageData() const = 0;
	virtual int getImageWidth() const { return mWidth; }
	virtual int getImageHeight() const { return mHeight; }
	virtual void use(int framebufferTarget = GL_FRAMEBUFFER) const { glBindFramebuffer(framebufferTarget, mFBO); }

protected:
	FBO mFBO;
	int mWidth;
	int mHeight;
};

#endif