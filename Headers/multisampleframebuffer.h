#ifndef MULTISAMPLE_FRAMEBUFFER_H
#define MULTISAMPLE_FRAMEBUFFER_H

#include <glad/glad.h>
#include "OpenGL Wrappers/FBO.h"
#include "OpenGL Wrappers/TEX.h"
#include "OpenGL Wrappers/RBO.h"
#include <vector>
#include "framebuffer.h"
#include "standardframebuffer.h"

// A framebuffer that has a color texture and depth/stencil rbo
class MultisampleFramebuffer : public Framebuffer {
public:
	MultisampleFramebuffer(int width, int height);
	std::vector<GLubyte> getImageData() const;

private:
	StandardFramebuffer mIntermediateFramebuffer;
	TEX mMultisampleColorTex;
	RBO mDepthStencilRBO;
};

#endif