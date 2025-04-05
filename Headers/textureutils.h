#ifndef TEXTURE_UTILS_H
#define TEXTURE_UTILS_H

#include <string>
#include "texturetype.h"
#include "modeltexture.h"

class TEX;
class TextureData;

namespace TextureUtils {
	struct DefaultTextures {
		ModelTexture diffuse{ "images/defaultdiffuse.png", TextureType::diffuse };
		ModelTexture specular{ "images/black.png", TextureType::diffuse };
		ModelTexture emission{ "images/black.png", TextureType::diffuse };
		ModelTexture reflection{ "images/black.png", TextureType::diffuse };
	};
	TEX texture2DFromFile(const std::string& filePath);
}

#endif