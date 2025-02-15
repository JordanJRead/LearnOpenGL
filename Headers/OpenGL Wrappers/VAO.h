#pragma once
#include "glad/glad.h"
class VAO {
public:
	unsigned int mID;
	bool mHasMoved{ false };

	// Move constructor
	VAO(VAO&& other) noexcept {
		mID = other.mID;
		other.mHasMoved = true;
	}

	VAO& operator=(const VAO&) = delete; // copy assignment
	VAO& operator=(VAO&&)      = delete; // move assignment

	// regular constructor and destructor
	  VAO() {                 glGenVertexArrays   (1, &mID); }
	 ~VAO() { if (!mHasMoved) glDeleteVertexArrays(1, &mID); }
};