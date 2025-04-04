#ifndef EBO_H
#define EBO_H

#include <glad/glad.h>
class EBO {
public:
	unsigned int mID;
	bool mHasMoved{ false };

	// Move constructor
	EBO(EBO&& other) noexcept {
		mID = other.mID;
		other.mHasMoved = true;
	}

	EBO& operator=(const EBO&) = delete; // copy assignment
	EBO& operator=(EBO&&) = delete; // move assignment

	// regular constructor and destructor
	EBO() { glGenBuffers(1, &mID); }
	~EBO() { if (!mHasMoved) glDeleteBuffers(1, &mID); }

	operator unsigned int() const {
		return mID;
	}
};

#endif