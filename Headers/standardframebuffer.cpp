#include "standardframebuffer.h"
#include <vector>

StandardFramebuffer::StandardFramebuffer(int width, int height) {
	mWidth = width;
	mHeight = height;

	glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	glActiveTexture(GL_TEXTURE0);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glBindTexture(GL_TEXTURE_2D, mColorTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthStencilRBO);
}

int StandardFramebuffer::getImageWidth() const {
	int imageWidth;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &imageWidth);

	return imageWidth;
}

int StandardFramebuffer::getImageHeight() const {
	int imageHeight;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &imageHeight);

	return imageHeight;
}


std::vector<GLubyte> StandardFramebuffer::getImageData() const {

	glBindTexture(GL_TEXTURE_2D, mColorTexture);

	unsigned int byteCount{ static_cast<unsigned int>(getImageWidth()) * static_cast<unsigned int>(getImageHeight()) * 4 * sizeof(GLubyte) };
	std::vector<GLubyte> imageData(byteCount);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(imageData[0]));

	return imageData;
}