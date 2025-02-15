#pragma once
#include <string>
#include "glad/glad.h"

class Texture {
private:
	unsigned int textureFromFile(std::string_view imagePath);
public:
	enum Type {
		diffuse,
		specular
	};

	std::string  mPath;
	unsigned int mID;
	Type mType;

	Texture(const std::string& path, Type type);
};