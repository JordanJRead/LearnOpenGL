#pragma once

#include <string>
#include "modeltexture.h"

class TEX;
class TextureData;

namespace TextureUtils {
	struct DefaultTextures {
		ModelTexture diffuse{ "images/defaultdiffuse.png", ModelTexture::diffuse };
		ModelTexture specular{ "images/black.png", ModelTexture::diffuse };
		ModelTexture emission{ "images/black.png", ModelTexture::diffuse };
		ModelTexture reflection{ "images/black.png", ModelTexture::diffuse };
	};
	TEX texture2DFromFile(const std::string& filePath);
}