#include "OpenGL Wrappers/TEX.h"
#include <vector>
#include "texture2d.h"
#include <string>
#include "textureutils.h"
#include "texture2dmanager.h"
#include <string_view>

/// <summary>
/// Takes in a file name and returns the index of that loaded texture
/// </summary>
int Texture2DManager::loadTexture(const std::string& fileName) {
	std::string path{ "images/" + fileName };
	for (int i = 0; i < mTextures.size(); ++i) {
		if (mTextures[i].mPath == path) {
			return i;
		}
	}
	if (fileName == "grass" || fileName == "grass.png") {
		mTextures.emplace_back(path, true); //todo what if path is wrong?
	}
	else {
		mTextures.emplace_back(path, false); //todo what if path is wrong?
	}
	return mTextures.size() - 1;
}

const Video& Texture2DManager::getVideoWithIndex(int index) const {
	const Video* firstVideo = &(mVideos[0]); // could be wrong, but will probably get corrected in the loop
	for (const Video& video : mVideos) {
		if (video.mVideoIndex == index) {
			return video;
		}
		if (video.mVideoIndex == 0) {
			firstVideo = &video;
		}
	}
	return *firstVideo;
}

const TEX& Texture2DManager::getTexture(int i, TextureUtils::Type type) const {
	if (i < 0 || i >= mTextures.size()) {
		switch (type) {
		case TextureUtils::Type::diffuse:
			return mDefaultTextures.diffuse.mTex;
		case TextureUtils::Type::specular:
			return mDefaultTextures.specular.mTex;
		case TextureUtils::Type::emission:
			return mDefaultTextures.emission.mTex;
		case TextureUtils::Type::reflection:
			return mDefaultTextures.reflection.mTex;
		}
	}

	const Texture2D& texture{ mTextures[i] };

	std::string fileName{ texture.mPath.substr(texture.mPath.find_last_of('/') + 1)}; // fileName is xxxxx_##_.fegfeg
	if (fileName.substr(0, 5) == "video") {
		int videoIndex = std::stoi(
			texture.mPath.substr(
				texture.mPath.find_first_of('_') + 1,
				texture.mPath.find_last_of('_') - (texture.mPath.find_first_of('/') + 1)
			)
		);

		const Video& video{ getVideoWithIndex(videoIndex) };
		return video.getFrame();

	}
	return texture.mTex;
}

Texture2DManager::Texture2DManager() {
	std::string dir = "videos/";
	for (const auto& videoDirectory : std::filesystem::directory_iterator(dir)) {
		int videoIndex = std::stoi(
			videoDirectory.path().string().substr(videoDirectory.path().string().find_last_of('/') + 1)
		);

		Video video{ videoIndex, 60, true };
		mVideos.emplace_back(std::move(video));
	};
}