#ifndef MODEL_H
#define MODEL_H

#include "texture2d.h"
#include "mesh.h"
#include "textureutils.h"
#include "transform.h"

class Mesh;
class aiScene;
class aiNode;
class aiMesh;
class aiMaterial;
enum aiTextureType;

class Model {
public:
	bool mHasBorder{ false };
	bool mUsesDynamicEnvironment{ false };
	int mDynamicEnvironmentIndex{ -1 };
	Transform mTransform;
	Model(const std::string& path, const Transform& transform, bool usesDynamicEnvironment = false, bool hasBorder = false);
	const std::vector<Mesh>& getMeshes() const;

private:
	std::vector<Mesh> mMeshes;
	std::string mDirectory;

	void loadModel(const std::string& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<int> loadMaterialTextureIndices(aiMaterial* mat, aiTextureType type, TextureUtils::Type typeName);

};

#endif