#pragma once
#include "glad/glad.h"
class RBO {
public:
	unsigned int mID;
	bool mHasMoved{ false };

	// Move constructor
	RBO(RBO&& other) noexcept {
		mID = other.mID;
		other.mHasMoved = true;
	}

	RBO& operator=(const RBO&) = delete; // copy assignment
	RBO& operator=(RBO&&) = delete; // move assignment

	// regular constructor and destructor
	RBO() { glGenRenderbuffers(1, &mID); }
	~RBO() { if (!mHasMoved) glDeleteRenderbuffers(1, &mID); }

	operator unsigned int() const {
		return mID;
	}

	void recreate() {
		if (!mHasMoved) { glDeleteRenderbuffers(1, &mID); }
		glGenRenderbuffers(1, &mID);
	}
};