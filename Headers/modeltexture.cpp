#pragma once
#include <string>
#include <glad/glad.h>
#include "stb_image.h"
#include <iostream>
#include "modeltexture.h"
#include "OpenGL Wrappers/TEX.h"
#include "textureutils.h"
#include "texturetype.h"

ModelTexture::ModelTexture(const std::string& path, TextureType type)
	: mPath{ path }
	, mType{ type }
	, mTex{ TextureUtils::texture2DFromFile(path) } {
}