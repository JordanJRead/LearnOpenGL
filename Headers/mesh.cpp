#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <glad/glad.h>
#include "mesh.h"
#include "modeltexture.h"
#include <iostream>
#include "textureutils.h"

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

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<size_t>& textureIndices, float shininess)
	: mVertexCount{ static_cast<unsigned int>(indices.size()) }
	, mTextureIndices{ textureIndices }
	, mShininess{ shininess }
{
	setupMesh(vertices, indices);
}

unsigned int Mesh::getFirstDiffuseMap(const std::vector<ModelTexture>& textures, const TextureUtils::DefaultTextures& defaultTextures) const {
	for (size_t textureIndex : mTextureIndices) {
		const ModelTexture& texture = textures[textureIndex];
		if (texture.mType == ModelTexture::diffuse) {
			return texture.mTex.mID;
		}
	}
	return defaultTextures.diffuse.mTex.mID;
}
unsigned int Mesh::getFirstSpecularMap(const std::vector<ModelTexture>& textures, const TextureUtils::DefaultTextures& defaultTextures) const {
	for (size_t textureIndex : mTextureIndices) {
		const ModelTexture& texture = textures[textureIndex];
		if (texture.mType == ModelTexture::specular) {
			return texture.mTex.mID;
		}
	}
	return defaultTextures.specular.mTex.mID;
}
unsigned int Mesh::getFirstEmissionMap(const std::vector<ModelTexture>& textures, const TextureUtils::DefaultTextures& defaultTextures) const {
	for (size_t textureIndex : mTextureIndices) {
		const ModelTexture& texture = textures[textureIndex];
		if (texture.mType == ModelTexture::emission) {
			return texture.mTex.mID;
		}
	}
	return defaultTextures.emission.mTex.mID;
}
unsigned int Mesh::getFirstReflectionMap(const std::vector<ModelTexture>& textures, const TextureUtils::DefaultTextures& defaultTextures) const {
	for (size_t textureIndex : mTextureIndices) {
		const ModelTexture& texture = textures[textureIndex];
		if (texture.mType == ModelTexture::reflection) {
			return texture.mTex.mID;
		}
	}
	return defaultTextures.reflection.mTex.mID;
}