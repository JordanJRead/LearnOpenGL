#ifndef MESH_H
#define MESH_H

#include <vector>
#include "structs.h"

class Texture;

class Mesh {
private:
	std::vector<size_t> mTextureIndices;
	void setupMesh(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices);

public:
	unsigned int mVAO, mVBO, mEBO;
	unsigned int mVertexCount;
	float mShininess;

	Mesh(const std::vector<Vertex>& vertices, const std::vector<size_t>& indices, const std::vector<size_t>& textureIndices, float shininess);

	int getFirstDiffuse(const std::vector<Texture>& textures) const;
	int getFirstSpecular(const std::vector<Texture>& textures) const;
};

#endif