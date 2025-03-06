#ifndef MESH_H
#define MESH_H

#include <vector>
#include "structs.h"
#include "OpenGL Wrappers/VAO.h"
#include "OpenGL Wrappers/VBO.h"
#include "OpenGL Wrappers/EBO.h"

class ModelTexture;

class Mesh {
private:
	std::vector<size_t> mTextureIndices;
	void setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

public:
	VAO mVAO;
	VBO mVBO;
	EBO mEBO;
	unsigned int mVertexCount;
	float mShininess;

	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<size_t>& textureIndices, float shininess);

	int getFirstDiffuse(const std::vector<ModelTexture>& textures) const;
	int getFirstSpecular(const std::vector<ModelTexture>& textures) const;
};

#endif