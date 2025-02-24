#pragma once
#include <string>
#include "glad/glad.h"
#include "stb_image.h"
#include <iostream>
#include "texture.h"
#include "OpenGL Wrappers/TEX.h"

void Texture::textureFromFile(std::string_view imagePath) {
	glBindTexture(GL_TEXTURE_2D, mTex.mID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	int width, height, channelCount;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imagePath.data(), &width, &height, &channelCount, 0);

	if (data) {
		auto internalFormat{ channelCount == 3 ? GL_RGB : GL_RGBA };
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else {
		std::cerr << "Failed to load texture data " << imagePath << "\n";
	}
}

enum Type {
	diffuse,
	specular
};

Texture::Texture(const std::string& path, Type type) : mPath{ path }, mType{ type } {
	textureFromFile(path);
}