#ifndef VERTEX_ARRAY_WRAPPER
#define VERTEX_ARRAY_WRAPPER

#include <vector>
#include <glad/glad.h>

class VertexArrayWrapper {
	unsigned int m_id;
public:
	VertexArrayWrapper(const std::vector<float>& vertices) {
		glGenVertexArrays(1, &m_id);
		glBindVertexArray(m_id);

		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
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
		glBindVertexArray(m_id);
	}
};

#endif
