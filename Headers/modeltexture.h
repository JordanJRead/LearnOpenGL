#pragma once
#include <string>
#include <glad/glad.h>
#include "OpenGL Wrappers/TEX.h"

class ModelTexture {
public:
	enum Type {
		diffuse,
		specular,
		emission,
		reflection
	};

	std::string  mPath;
	TEX mTex;
	Type mType;

	ModelTexture(const std::string& path, Type type);
};