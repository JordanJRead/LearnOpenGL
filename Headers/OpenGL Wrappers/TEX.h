#pragma once
#include "glad/glad.h"
class TEX {
public:
	unsigned int mID;
	bool mHasMoved{ false };

	// Move constructor
	TEX(TEX&& other) noexcept {
		mID = other.mID;
		other.mHasMoved = true;
	}
	TEX& operator=(TEX&&) = delete; // move assignment
	//TEX& operator=(TEX&& other) noexcept {
	//	other.mHasMoved = true;
	//	if (!mHasMoved) {
	//		glDeleteTextures(1, &mID);
	//	}
	//	mID = other.mID;
	//}

	TEX& operator=(const TEX&) = delete; // copy assignment

	// regular constructor and destructor
	TEX() { glGenTextures(1, &mID); }
	~TEX() { if (!mHasMoved) glDeleteTextures(1, &mID); }

	void recreate() {
		if (!mHasMoved) { glDeleteTextures(1, &mID); }
		glGenTextures(1, &mID);
	}

	operator unsigned int() const {
		return mID;
	}
};