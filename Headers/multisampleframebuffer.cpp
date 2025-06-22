#include "multisampleframebuffer.h"
#include <vector>

MultisampleFramebuffer::MultisampleFramebuffer(int width, int height)
	: Framebuffer{ width, height }
	, mIntermediateFramebuffer{ width, height }
{
	this->use();

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, mMultisampleColorTex);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, width, height, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilRBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, mMultisampleColorTex, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthStencilRBO);
}

std::vector<GLubyte> MultisampleFramebuffer::getImageData() const {
	this->use(GL_READ_FRAMEBUFFER);
	mIntermediateFramebuffer.use(GL_DRAW_FRAMEBUFFER);
	glBlitFramebuffer(0, 0, mWidth, mHeight, 0, 0, mIntermediateFramebuffer.getImageWidth(), mIntermediateFramebuffer.getImageHeight(), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
	this->use();
	return mIntermediateFramebuffer.getImageData();
}