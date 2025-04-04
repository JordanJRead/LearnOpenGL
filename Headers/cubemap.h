#pragma once

#include <vector>
#include <string>
#include "OpenGL Wrappers/TEX.h"
#include <cassert>
#include "texturedata.h"
#include <array>
#include <cstddef>

class CubeMap {
public:
	TEX mTEX;

	CubeMap(const std::vector<std::string>& filePaths) {
		assert(filePaths.size() == 6);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mTEX);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		for (int i{ 0 }; i < 6; ++i) {
			TextureData texData{ filePaths[i] };
			auto format{ texData.getChannelCount() == 3 ? GL_RGB : GL_RGBA };
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, texData.getWidth(), texData.getHeight(), 0, format, GL_UNSIGNED_BYTE, texData.getData());
		}
	}
};