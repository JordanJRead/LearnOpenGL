#pragma once
#include <string>
#include "glad/glad.h"
#include "OpenGL Wrappers/TEX.h"

class Texture {
private:
	void textureFromFile(std::string_view imagePath);
public:
	enum Type {
		diffuse,
		specular
	};

	std::string  mPath;
	TEX mTex;
	Type mType;

	Texture(const std::string& path, Type type);
};