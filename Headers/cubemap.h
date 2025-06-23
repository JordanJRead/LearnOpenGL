#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <vector>
#include <string>
#include "OpenGL Wrappers/TEX.h"
#include <cassert>
#include "imagedata.h"
#include <array>
#include <cstddef>

class CubeMap {
public:
	TEX mTEX;

	CubeMap(const std::vector<std::string>& filePaths, bool srgba = false) {
		assert(filePaths.size() == 6);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mTEX);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		for (int i{ 0 }; i < 6; ++i) {
			ImageData imageData{ filePaths[i] };
			int format{ imageData.getChannelCount() == 3 ? GL_RGB : GL_RGBA };
			int internalFormat{ format };
			if (srgba) {
				if (format == GL_RGB)
					internalFormat = GL_SRGB;
				else if (format == GL_RGBA)
					internalFormat = GL_SRGB_ALPHA;
			}
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, imageData.getWidth(), imageData.getHeight(), 0, format, GL_UNSIGNED_BYTE, imageData.getData());
		}
	}
};

#endif