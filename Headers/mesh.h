#ifndef MESH_H
#define MESH_H

#include <vector>
#include "structs.h"
#include "glad/glad.h"

class Model;

class Mesh {
private:
	unsigned int mVBO, mEBO;
	const Model* mParentModel;
	std::vector<size_t> mTextureIndices;

	Mesh& operator=(const Mesh&) = delete;
	Mesh& operator=(Mesh&&) = delete;
	Mesh(const Mesh&) = delete;
	Mesh(Mesh&&) = delete;

	void setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

public:
	unsigned int mVAO;
	unsigned int mVertexCount;
	float mShininess;

	Mesh(const Model* model, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, std::vector<size_t> textureIndices, float shininess);

	int getFirstDiffuse() const;
	int getFirstSpecular() const;

	~Mesh() {
		glDeleteVertexArrays(1, &mVAO);
		glDeleteBuffers(1, &mVBO);
		glDeleteBuffers(1, &mEBO);
	}
};

#endif