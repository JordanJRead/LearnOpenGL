#include "OpenGL Wrappers/TEX.h"
#include <vector>
#include "texture2d.h"
#include <string>
#include "textureutils.h"
#include "texture2dmanager.h"

/// <summary>
/// Takes in a file name and returns the index of that loaded texture
/// </summary>
int Texture2DManager::loadTexture(const std::string& fileName, bool srgba) {
	std::string path{ "images/" + fileName };
	for (int i = 0; i < mTextures.size(); ++i) {
		if (mTextures[i].mPath == path && srgba == mTextures[i].mSRGBA) {
			return i;
		}
	}
	mTextures.emplace_back(path, srgba); //todo what if path is wrong?
	return mTextures.size() - 1;
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

	return mTextures[i].mTex;
}