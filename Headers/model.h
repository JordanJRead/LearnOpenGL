#ifndef MODEL_H
#define MODEL_H
#include <assimp/scene.h>
#include "glad/glad.h"
#include "structs.h"
#include "mesh.h"
#include "texture.h"

struct MeshInfo;

class Model {
private:
	bool hasMoved{ false };
	friend class Mesh;
	Model& operator=(const Model&) = delete;
	Model& operator=(Model&& other) {
		other.hasMoved = true;

	}
	Model(const Model&) = delete;
	Model(Model&&) = delete;

	std::vector<Mesh> mMeshes;
	std::string mDirectory;
	std::vector<Texture> mTextures;

	void loadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	MeshInfo processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<size_t> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);

public:
	glm::mat4 mModelMatrix;
	bool mHasBorder;
	Model(const std::string& path, const Transform& transform, bool hasBorder);
	const std::vector<Mesh>& getMeshes() const;
	~Model() {
		if (!hasMoved) {
			for (const Texture& texture : mTextures) {
				glDeleteTextures(1, &(texture.mID));
			}
		}
	}
};

#endif