#ifndef MATRIX_UNIFORM_BUFFER_H
#define MATRIX_UNIFORM_BUFFER_H

#include "OpenGL Wrappers/BUF.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class MatrixUniformBuffer {
	BUF mBuffer;
	int mBindingIndex;

public:
	MatrixUniformBuffer(int bindingIndex) : mBindingIndex{bindingIndex} {
		glBindBuffer(GL_UNIFORM_BUFFER, mBuffer);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, mBindingIndex, mBuffer);
	}

	void use() {
		glBindBuffer(GL_UNIFORM_BUFFER, mBuffer);
		glBindBufferBase(GL_UNIFORM_BUFFER, mBindingIndex, mBuffer);
	}

	void setViewMatrix(const glm::mat4& view) {
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
	}

	void setProjectionMatrix(const glm::mat4& projection) {
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
	}
};

#endif