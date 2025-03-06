#pragma once

#include <string>
class TEX;
class TextureData;

namespace TextureUtils {
	TEX texture2DFromFile(const std::string& filePath);
}