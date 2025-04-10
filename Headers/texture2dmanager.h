#ifndef TEXTURE_2D_MANAGER_H
#define TEXTURE_2D_MANAGER_H

#include "OpenGL Wrappers/TEX.h"
#include <vector>
#include "texture2d.h"
#include <string>
#include "textureutils.h"

class Texture2DManager {
private:
	std::vector<Texture2D> mTextures;
	TextureUtils::DefaultTextures2D mDefaultTextures;

public:
	int loadTexture(const std::string& path);
	const TEX& getTexture(int i, TextureUtils::Type type) const;
};

#endif