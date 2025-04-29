#include <string>
#include <glad/glad.h>
#include "stb_image.h"
#include <iostream>
#include "texture2d.h"
#include "OpenGL Wrappers/TEX.h"
#include "textureutils.h"

Texture2D::Texture2D(const std::string& path, bool clamp)
	: mPath{ path }
	, mTex{ TextureUtils::texture2DFromFile(path, clamp) } {
}