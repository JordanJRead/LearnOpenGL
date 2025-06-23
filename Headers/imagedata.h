#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include "stb_image.h"
#include <string>

class ImageData {
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

	ImageData(const std::string& imagePath) {
		stbi_set_flip_vertically_on_load(true);
		mData = stbi_load(imagePath.data(), &mWidth, &mHeight, &mChannelCount, 0);
	}
	
	// Move ctor
	ImageData(ImageData&& other) noexcept {
		mData = other.mData;
		other.mData = nullptr;

		mWidth = other.mWidth;
		mHeight = other.mHeight;
		mChannelCount = other.mChannelCount;
	}

	// Copy ctor
	ImageData(const ImageData& other) = delete;

	// Copy assign
	ImageData& operator=(const ImageData& other) = delete;

	// Move assign
	ImageData& operator=(ImageData&& other) = delete;

	~ImageData() {
		stbi_image_free(mData);
	}
};

#endif