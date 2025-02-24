#pragma once
#include "glad/glad.h"
class VBO {
public:
	unsigned int mID;
	bool mHasMoved{ false };

	// Move constructor
	VBO(VBO&& other) noexcept {
		mID = other.mID;
		other.mHasMoved = true;
	}

	VBO& operator=(const VBO&) = delete; // copy assignment
	VBO& operator=(VBO&&) = delete; // move assignment

	// regular constructor and destructor
	VBO() { glGenBuffers(1, &mID); }
	~VBO() { if (!mHasMoved) glDeleteBuffers(1, &mID); }

	operator unsigned int() const {
		return mID;
	}
};