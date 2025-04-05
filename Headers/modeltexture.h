#ifndef MODEL_TEXTURE_H
#define MODEL_TEXTURE_H

#include <string>
#include <glad/glad.h>
#include "OpenGL Wrappers/TEX.h"
#include "texturetype.h"

class ModelTexture {
public:
	std::string  mPath;
	TEX mTex;
	TextureType mType;

	ModelTexture(const std::string& path, TextureType type);
};

#endif