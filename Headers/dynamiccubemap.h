#pragma once
#include <vector>
#include <string>
#include "OpenGL Wrappers/TEX.h"
#include <cassert>
#include "texturedata.h"
#include <array>
#include <cstddef>

class DynamicCubeMap {
public:
	TEX mTEX;

	void setFace(const TEX& tex, int faceIndex) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, mTEX);
		glBindTexture(GL_TEXTURE_2D, tex);

		int imageWidth;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &imageWidth);

		int imageHeight;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &imageHeight);

		unsigned int byteCount{ static_cast<unsigned int>(imageWidth) * static_cast<unsigned int>(imageHeight) * 4 * sizeof(GLubyte) };
		GLubyte* imageData{ new GLubyte[byteCount]{} };
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

		delete[] imageData;
	}

	DynamicCubeMap() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mTEX);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
};