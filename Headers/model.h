#ifndef MODEL_H
#define MODEL_H
#include "mesh.h"
#include <assimp/scene.h>

class Model {
private:
	std::vector<Mesh> mMeshes;
	std::string mDirectory;
	std::vector<Texture> mLoadedTextures;

	void loadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	unsigned int textureFromFile(std::string_view imagePath);

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);

public:
	glm::mat4 mModel;
	Model(const std::string& path, const Transform& transform);
	//void draw(Shader& shader);
	const std::vector<Mesh>& getMeshes() const;
};

#endif