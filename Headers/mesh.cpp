#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "shader.h"
#include <glad/glad.h>
#include "structs.h"
#include "mesh.h"

void Mesh::setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glBindVertexArray(0);
}

Mesh::Mesh(const std::vector<Vertex>& vertices, std::vector<unsigned int> indices, std::vector<Texture> _textures, float _shininess)
	: mVertexCount{ static_cast<unsigned int>(indices.size()) }
	, mTextures{ _textures }
	, mShininess{ _shininess }
{
	setupMesh(vertices, indices);
}
//void Mesh::draw(Shader& shader) {
	//size_t diffuseCount{ 0 };
	//size_t specularCount{ 0 };
	//for (size_t i{ 0 }; i < textures.size(); ++i) {
	//	glActiveTexture(GL_TEXTURE0 + i);
	//	std::string number;
	//	std::string name{ textures[i].type };
	//	if (name == "texture_diffuse") {
	//		number = std::to_string(diffuseCount++);
	//	}
	//	else if (name == "texture_specular") {
	//		number = std::to_string(specularCount++);
	//	}
	//	shader.setFloat(("material." + name + number).c_str(), i);
	//	glBindTexture(GL_TEXTURE_2D, textures[i].id);
	//}
	//glActiveTexture(GL_TEXTURE0);

	//glBindVertexArray(VAO);
	//glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
//}

int Mesh::getFirstDiffuse() const {
	for (const Texture& texture : mTextures) {
		if (texture.type == TextureType::diffuse) {
			return texture.id;
		}
	}
	return -1;
}
int Mesh::getFirstSpecular() const {
	for (const Texture& texture : mTextures) {
		if (texture.type == TextureType::specular) {
			return texture.id;
		}
	}
	return -1;
}