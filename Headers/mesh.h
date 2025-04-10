#ifndef MESH_H
#define MESH_H

#include <vector>
#include "structs.h"
#include "OpenGL Wrappers/VAO.h"
#include "OpenGL Wrappers/BUF.h"
#include "OpenGL Wrappers/EBO.h"
#include "textureutils.h"

class Texture2DManager;
class Texture2D;

class Mesh {
private:
	TextureUtils::Texture2DIndices mTexture2DIndices;
	void setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

public:
	VAO mVAO;
	BUF mVBO;
	EBO mEBO;
	unsigned int mVertexCount;
	float mShininess;

	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const TextureUtils::Texture2DIndices& texture2DIndices, float shininess);

	const TEX& getFirstDiffuseMap() const;
	const TEX& getFirstSpecularMap() const;
	const TEX& getFirstEmissionMap() const;
	const TEX& getFirstReflectionMap() const;
};

#endif