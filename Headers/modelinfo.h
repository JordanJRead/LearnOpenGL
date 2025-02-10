#ifndef VERTEX_ARRAY_WRAPPER
#define VERTEX_ARRAY_WRAPPER

#include <vector>
#include <glad/glad.h>
#include "structs.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ModelInfo {
	unsigned int VAO;
	unsigned int VBO;
public:
	glm::mat4 model;
	int vertexCount;
	ModelInfo() = default;
	ModelInfo(const std::vector<float>& vertices, const Transform& transform) {
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

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

		model = glm::mat4(1.0);
		model = glm::translate(model, transform.pos);
		model = glm::rotate(model, transform.rotation.z, { 0, 0, 1 });
		model = glm::rotate(model, transform.rotation.y, { 0, 1, 0 });
		model = glm::rotate(model, transform.rotation.x, { 1, 0, 0 });
		model = glm::scale(model, transform.scale);

		vertexCount = vertices.size() / 8;
	}

	void updateTransform(const Transform& transform) {
		model = glm::mat4(1.0);
		model = glm::translate(model, transform.pos);
		model = glm::rotate(model, transform.rotation.x, { 1, 0, 0 });
		model = glm::rotate(model, transform.rotation.y, { 0, 1, 0 });
		model = glm::rotate(model, transform.rotation.z, { 0, 0, 1 });
		model = glm::scale(model, transform.scale);
	}

	void use() const {
		glBindVertexArray(VAO);
	}

	~ModelInfo() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
};

#endif
