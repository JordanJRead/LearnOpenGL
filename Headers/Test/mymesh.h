#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../shader.h"

struct MyMeshTexture {
	unsigned int id;
	std::string type;
	std::string path;
};

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

class MyMesh {
private:
	unsigned int VBO, VAO, EBO;
	std::vector<MyMeshTexture> textures;
	int vertexCount;

	void loadVertices(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

public:
	MyMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<MyMeshTexture>& _textures)
		: textures{ _textures }
		, vertexCount{ indices.size()} {
		loadVertices(vertices, indices);
	}

	void setTextures(const Shader& shader) {
		glUseProgram(shader.ID);
		int currDiffuse{ 0 };
		int currSpecular{ 0 };

		for (size_t i{ 0 }; i < textures.size(); ++i) {
			glActiveTexture(GL_TEXTURE0 + i);
			std::string name;
			std::string number;
			if (textures[i].type == "texture_diffuse") {
				name = textures[i].type;
				number = std::to_string(currDiffuse++);
			}
			else if (textures[i].type == "texture_specular") {
				name = textures[i].type ;
				number = std::to_string(currSpecular++);
			}
			shader.setFloat("material." + name + number, i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
	}

	void draw(const Shader& shader) {
		setTextures(shader);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
};

#endif