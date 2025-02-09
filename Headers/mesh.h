#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "shader.h"
#include <glad/glad.h>

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct MeshTexture {
	unsigned int id;
	std::string type; // enum?
	std::string path;
};

class Mesh {
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh(const std::vector<Vertex>& vertices) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

		glBindVertexArray(0);
	}

public:
	std::vector<unsigned int> indices;
	std::vector<MeshTexture> textures;

	// NOTE i removed 'vertices' member variable
	Mesh(const std::vector<Vertex>& vertices, std::vector<unsigned int> _indices, std::vector<MeshTexture> _textures)
		: indices{ _indices }
		, textures{ _textures }
	{
		setupMesh(vertices);
	}
	void draw(Shader& shader) {
		size_t diffuseCount{ 0 };
		size_t specularCount{ 0 };
		for (size_t i{ 0 }; i < textures.size(); ++i) {
			glActiveTexture(GL_TEXTURE0 + i);
			std::string number;
			std::string name{ textures[i].type };
			if (name == "texture_diffuse") {
				number = std::to_string(diffuseCount++);
			}
			else if (name == "texture_specular") {
				number = std::to_string(specularCount++);
			}
			shader.setFloat(("material." + name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
};

#endif