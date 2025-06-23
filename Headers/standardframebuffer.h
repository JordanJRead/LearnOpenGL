#ifndef STANDARD_FRAMEBUFFER_H
#define STANDARD_FRAMEBUFFER_H

#include <glad/glad.h>
#include "OpenGL Wrappers/FBO.h"
#include "OpenGL Wrappers/TEX.h"
#include "OpenGL Wrappers/RBO.h"
#include <vector>
#include "framebuffer.h"

// A framebuffer that has a color texture and depth/stencil rbo
class StandardFramebuffer : public Framebuffer {
public:
	StandardFramebuffer(int width, int height);
	std::vector<GLubyte> getImageData() const;
	const TEX& getColorTex() const;

private:
	TEX mColorTEX;
	RBO mDepthStencilRBO;
};

#endif