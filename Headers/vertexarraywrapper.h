#ifndef VERTEX_ARRAY_WRAPPER
#define VERTEX_ARRAY_WRAPPER

#include <vector>
#include <glad/glad.h>

class VertexArrayWrapper {
	unsigned int mVAO;
	unsigned int mVBO;
public:
	VertexArrayWrapper& operator=(const VertexArrayWrapper&) = delete;
	VertexArrayWrapper& operator=(VertexArrayWrapper&&) = delete;
	VertexArrayWrapper(const VertexArrayWrapper&) = delete;
	VertexArrayWrapper(VertexArrayWrapper&&) = delete;

	VertexArrayWrapper(const std::vector<float>& vertices) {
		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		glGenBuffers(1, &mVBO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void use() {
		glBindVertexArray(mVAO);
	}

	~VertexArrayWrapper() {
		glDeleteVertexArrays(1, &mVAO);
		glDeleteBuffers(1, &mVBO);
	}
};

#endif
