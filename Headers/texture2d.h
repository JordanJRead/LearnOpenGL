#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include <string>
#include <glad/glad.h>
#include "OpenGL Wrappers/TEX.h"

class Texture2D {
public:
	std::string mPath;
	TEX mTex;

	Texture2D(const std::string& path, bool clamp = false);
};

#endif