#ifndef MESH_H
#define MESH_H

#include <vector>
#include "structs.h"
#include "OpenGL Wrappers/VAO.h"
#include "OpenGL Wrappers/BUF.h"
#include "OpenGL Wrappers/EBO.h"
#include "textureutils.h"

class ModelTexture;

class Mesh {
private:
	std::vector<size_t> mTextureIndices;
	void setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

public:
	VAO mVAO;
	BUF mVBO;
	EBO mEBO;
	unsigned int mVertexCount;
	float mShininess;

	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<size_t>& textureIndices, float shininess);

	unsigned int getFirstDiffuseMap(const std::vector<ModelTexture>& textures, const TextureUtils::DefaultTextures& defaultTextures) const;
	unsigned int getFirstSpecularMap(const std::vector<ModelTexture>& textures, const TextureUtils::DefaultTextures& defaultTextures) const;
	unsigned int getFirstEmissionMap(const std::vector<ModelTexture>& textures, const TextureUtils::DefaultTextures& defaultTextures) const;
	unsigned int getFirstReflectionMap(const std::vector<ModelTexture>& textures, const TextureUtils::DefaultTextures& defaultTextures) const;
};

#endif