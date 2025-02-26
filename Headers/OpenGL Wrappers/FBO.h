#pragma once
#include "glad/glad.h"
class FBO {
public:
	unsigned int mID;
	bool mHasMoved{ false };

	// Move constructor
	FBO(FBO&& other) noexcept {
		mID = other.mID;
		other.mHasMoved = true;
	}

	FBO& operator=(const FBO&) = delete; // copy assignment
	FBO& operator=(FBO&&) = delete; // move assignment

	// regular constructor and destructor
	FBO() { glGenFramebuffers(1, &mID); }
	~FBO() { if (!mHasMoved) glDeleteFramebuffers(1, &mID); }

	operator unsigned int() const {
		return mID;
	}

	void recreate() {
		if (!mHasMoved) { glDeleteFramebuffers(1, &mID); }
		glGenFramebuffers(1, &mID);
	}
};