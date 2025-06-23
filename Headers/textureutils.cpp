#include "OpenGL Wrappers/TEX.h"
#include <string>
#include "textureutils.h"
#include "imagedata.h"
#include <iostream>
#include "texture2d.h"

TEX TextureUtils::texture2DFromFile(const std::string& filePath, bool srgba) {
	TEX tex;
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	ImageData imageData{ filePath };

	if (imageData.getData()) {
		int format{ imageData.getChannelCount() == 3 ? GL_RGB : GL_RGBA };
		int internalFormat{ format };
		if (srgba) {
			if (format == GL_RGB)
				internalFormat = GL_SRGB;
			else if (format == GL_RGBA)
				internalFormat = GL_SRGB_ALPHA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, imageData.getWidth(), imageData.getHeight(), 0, format	, GL_UNSIGNED_BYTE, imageData.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cerr << "Failed to load texture data " << filePath << "\n";
	}
	return tex;
}