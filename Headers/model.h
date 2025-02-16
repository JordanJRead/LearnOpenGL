#ifndef MODEL_H
#define MODEL_H

#include "texture.h"
#include "mesh.h"

class Mesh;
class aiScene;
class aiNode;
class aiMesh;
class aiMaterial;
enum aiTextureType;

class Model {
private:
	std::vector<Mesh> mMeshes;
	std::string mDirectory;

	void loadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	unsigned int textureFromFile(std::string_view imagePath);

	std::vector<size_t> loadMaterialTextureIndices(aiMaterial* mat, aiTextureType type, Texture::Type typeName);

public:
	std::vector<Texture> mLoadedTextures;
	glm::mat4 mModel;
	Model(const std::string& path, const Transform& transform);
	const std::vector<Mesh>& getMeshes() const;
};

#endif