#ifndef MESH_H
#define MESH_H

#include <vector>
#include "structs.h"

class Texture;

class Mesh {
private:
	std::vector<Texture> mTextures;
	void setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

public:
	unsigned int mVAO, mVBO, mEBO;
	unsigned int mVertexCount;
	float mShininess;

	Mesh(const std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture> textures, float shininess);

	int getFirstDiffuse() const;
	int getFirstSpecular() const;
};

#endif