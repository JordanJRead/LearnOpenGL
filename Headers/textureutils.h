#ifndef TEXTURE_UTILS_H
#define TEXTURE_UTILS_H

#include <string>
#include "texture2d.h"
#include "vector"

class TEX;
class TextureData;

namespace TextureUtils {

	enum class Type {
		diffuse,
		specular,
		emission,
		reflection,
		skybox
	};

	struct DefaultTextures2D {
		Texture2D diffuse{ "images/defaultdiffuse.png" };
		Texture2D specular{ "images/black.png" };
		Texture2D emission{ "images/black.png" };
		Texture2D reflection{ "images/black.png" };
	};

	struct Texture2DIndices {
		std::vector<int> diffuse;
		std::vector<int> specular;
		std::vector<int> emission;
		std::vector<int> reflection;
	};

	TEX texture2DFromFile(const std::string& filePath);
}

#endif