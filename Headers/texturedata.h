#pragma once
#include "stb_image.h"
#include <string>

class TextureData {
private:
	unsigned char* mData;
	int mWidth;
	int mHeight;
	int mChannelCount;

public:
	unsigned char* getData() const { return mData; }
	int getWidth() const { return mWidth; }
	int getHeight() const { return mHeight; }
	int getChannelCount() const { return mChannelCount; }

	TextureData(const std::string& imagePath) {
		stbi_set_flip_vertically_on_load(true);
		mData = stbi_load(imagePath.data(), &mWidth, &mHeight, &mChannelCount, 0);
	}
	
	// Move ctor
	TextureData(TextureData&& other) noexcept {
		mData = other.mData;
		other.mData = nullptr;

		mWidth = other.mWidth;
		mHeight = other.mHeight;
		mChannelCount = other.mChannelCount;
	}

	// Copy ctor
	TextureData(const TextureData& other) = delete;

	// Copy assign
	TextureData& operator=(const TextureData& other) = delete;

	// Move assign
	TextureData& operator=(TextureData&& other) = delete;

	~TextureData() {
		stbi_image_free(mData);
	}
};