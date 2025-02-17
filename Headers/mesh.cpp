#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <glad/glad.h>
#include "mesh.h"
#include "texture.h"
#include <iostream>

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

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<size_t>& textureIndices, float shininess)
	: mVertexCount{ static_cast<unsigned int>(indices.size()) }
	, mTextureIndices{ textureIndices }
	, mShininess{ shininess }
{
	setupMesh(vertices, indices);
}

int Mesh::getFirstDiffuse(const std::vector<Texture>& textures) const {
	for (size_t textureIndex : mTextureIndices) {
		const Texture& texture = textures[textureIndex];
		if (texture.mType == Texture::diffuse) {
			return texture.mTex.mID; // TODO return TEX instead maybe?
		}
	}
	return -1;
}
int Mesh::getFirstSpecular(const std::vector<Texture>& textures) const {
	for (size_t textureIndex : mTextureIndices) {
		const Texture& texture = textures[textureIndex];
		if (texture.mType == Texture::specular) {
			return texture.mTex.mID;
		}
	}
	return -1;
}