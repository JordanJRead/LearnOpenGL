#pragma once
#include "glad/glad.h"
inline unsigned int textureFromFile(std::string_view imagePath) {
	unsigned int ID;
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
		std::cerr << "Failed to load texture data\n";
	}
	return ID;
}

class Texture {
private:
	bool hasMoved{ false };
public:
	Texture(const Texture&) = delete;
	Texture(Texture&& other) {
		other.hasMoved = true;
		mID = other.mID;
		mType = other.mType;
		mPath = other.mPath;
	}
	Texture& operator=(const Texture&) = delete;
	Texture& operator=(Texture&&) = delete;

	unsigned int mID;
	TextureType mType;
	std::string mPath;

	Texture(const std::string& path, TextureType type)
		: mPath{ path }// relative path?
		, mType{ type }
	{
		mID = textureFromFile(path);
	}

	~Texture() {
		if (!hasMoved) {
			glDeleteTextures(1, &mID);
		}
	}
};