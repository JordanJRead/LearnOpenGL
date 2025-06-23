#pragma once
#include <vector>
#include <string>
#include "OpenGL Wrappers/TEX.h"
#include <cassert>
#include "imagedata.h"
#include <array>
#include <cstddef>
#include "framebuffer.h"

class DynamicCubeMap {
public:
	TEX mTEX;

	void setFace(const Framebuffer& framebuffer, int faceIndex) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, mTEX);
		std::vector<GLubyte> imageData{ framebuffer.getImageData() };
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex, 0, GL_RGBA, framebuffer.getImageWidth(), framebuffer.getImageHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, &(imageData[0]));
	}

	DynamicCubeMap() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mTEX);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
};