#ifndef BUF_H
#define BUF_H

#include <glad/glad.h>
class BUF {
public:
	unsigned int mID;
	bool mHasMoved{ false };

	// Move constructor
	BUF(BUF&& other) noexcept {
		mID = other.mID;
		other.mHasMoved = true;
	}

	BUF& operator=(const BUF&) = delete; // copy assignment
	BUF& operator=(BUF&&) = delete; // move assignment

	// regular constructor and destructor
	BUF() { glGenBuffers(1, &mID); }
	~BUF() { if (!mHasMoved) glDeleteBuffers(1, &mID); }

	operator unsigned int() const {
		return mID;
	}
};

#endif