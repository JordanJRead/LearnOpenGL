#include "OpenGL Wrappers/TEX.h"
#include <string>
#include "textureutils.h"
#include "texturedata.h"
#include <iostream>
#include "modeltexture.h"

TEX TextureUtils::texture2DFromFile(const std::string& filePath) {
	TEX tex;
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	TextureData texData{ filePath };

	if (texData.getData()) {
		auto internalFormat{ texData.getChannelCount() == 3 ? GL_RGB : GL_RGBA };
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texData.getWidth(), texData.getHeight(), 0, internalFormat, GL_UNSIGNED_BYTE, texData.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cerr << "Failed to load texture data " << filePath << "\n";
	}
	return tex;
}