#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <glad/glad.h>
#include "mesh.h"
#include "texture2d.h"
#include <iostream>
#include "textureutils.h"
#include "texture2dmanager.h"
extern Texture2DManager* gTexture2DManager;

void Mesh::setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
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

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const TextureUtils::Texture2DIndices& texture2DIndices, float shininess)
	: mVertexCount{ static_cast<unsigned int>(indices.size()) }
	, mTexture2DIndices{ texture2DIndices }
	, mShininess{ shininess }
{
	setupMesh(vertices, indices);
}

const TEX& Mesh::getFirstDiffuseMap() const {
	int textureIndex = mTexture2DIndices.diffuse.size() > 0 ? mTexture2DIndices.diffuse[0] : -1;
	return gTexture2DManager->getTexture(textureIndex, TextureUtils::Type::diffuse);
}
const TEX& Mesh::getFirstSpecularMap() const {
	int textureIndex = mTexture2DIndices.specular.size() > 0 ? mTexture2DIndices.specular[0] : -1;
	return gTexture2DManager->getTexture(textureIndex, TextureUtils::Type::specular);
}
const TEX& Mesh::getFirstEmissionMap() const {
	int textureIndex = mTexture2DIndices.emission.size() > 0 ? mTexture2DIndices.emission[0] : -1;
	return gTexture2DManager->getTexture(textureIndex, TextureUtils::Type::emission);
}
const TEX& Mesh::getFirstReflectionMap() const {
	int textureIndex = mTexture2DIndices.reflection.size() > 0 ? mTexture2DIndices.reflection[0] : -1;
	return gTexture2DManager->getTexture(textureIndex, TextureUtils::Type::reflection);
}