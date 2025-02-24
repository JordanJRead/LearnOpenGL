#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "structs.h"
#include "stb_image.h"
#include "model.h"
#include <iostream>
#include <glad/glad.h>
#include "texture.h"
#include <iostream>

void Model::loadModel(const std::string& path) {
	Assimp::Importer importer{};
	const aiScene* scene{ importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs) };

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << "'\n";
		return;
	}
	mDirectory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}
void Model::processNode(aiNode* node, const aiScene* scene) {
	for (size_t i{ 0 }; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		mMeshes.push_back(processMesh(mesh, scene));
	}

	for (size_t i{ 0 }; i < node->mNumChildren; ++i) {
		processNode(node->mChildren[i], scene);
	}
}
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<size_t> textureIndices;

	for (size_t vertI{ 0 }; vertI < mesh->mNumVertices; ++vertI) {
		Vertex vertex;
		glm::vec3 pos;
		pos.x = mesh->mVertices[vertI].x;
		pos.y = mesh->mVertices[vertI].y;
		pos.z = mesh->mVertices[vertI].z;
		vertex.pos = pos;

		glm::vec3 normal;
		normal.x = mesh->mNormals[vertI].x;
		normal.y = mesh->mNormals[vertI].y;
		normal.z = mesh->mNormals[vertI].z;
		vertex.normal = normal;

		if (mesh->mTextureCoords[0]) {
			glm::vec2 texCoords;
			texCoords.x = mesh->mTextureCoords[0][vertI].x;
			texCoords.y = mesh->mTextureCoords[0][vertI].y;
			vertex.texCoords = texCoords;
		}
		else {
			vertex.texCoords = glm::vec2(0, 0);
		}
		vertices.push_back(vertex);
	}

	for (size_t faceI{ 0 }; faceI < mesh->mNumFaces; ++faceI) {
		aiFace face = mesh->mFaces[faceI];
		for (size_t j{ 0 }; j < face.mNumIndices; ++j) {
			indices.push_back(face.mIndices[j]);
		}
	}
	float shininess{ 32 };
	if (mesh->mMaterialIndex >= 0) { // ?
		aiMaterial* material{ scene->mMaterials[mesh->mMaterialIndex] };
		std::vector<size_t> diffuseMapIndices = loadMaterialTextureIndices(material, aiTextureType_DIFFUSE, Texture::diffuse);
		textureIndices.insert(textureIndices.end(), diffuseMapIndices.begin(), diffuseMapIndices.end());
		std::vector<size_t> specularMapIndices = loadMaterialTextureIndices(material, aiTextureType_SPECULAR, Texture::specular);
		textureIndices.insert(textureIndices.end(), specularMapIndices.begin(), specularMapIndices.end());
		if (AI_SUCCESS != aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess)) {
			shininess = 32;
		}
	}
	return Mesh{ vertices, indices, textureIndices, shininess };
}

std::vector<size_t> Model::loadMaterialTextureIndices(aiMaterial* mat, aiTextureType type, Texture::Type typeName) {
	std::vector<size_t> textureIndices;
	for (size_t i{ 0 }; i < mat->GetTextureCount(type); ++i) {
		aiString texturePath;
		mat->GetTexture(type, i, &texturePath);
		bool alreadyLoaded{ false };
		for (size_t loadedTextureIndex{ 0 }; loadedTextureIndex < mLoadedTextures.size(); ++loadedTextureIndex) {
			const Texture& loadedTexture = mLoadedTextures[loadedTextureIndex];
			if (std::strcmp(loadedTexture.mPath.data(), (mDirectory + '/' + texturePath.C_Str()).data()) == 0) {
				textureIndices.push_back(loadedTextureIndex);
				alreadyLoaded = true;
				break;
			}
		}
		if (!alreadyLoaded) {
			mLoadedTextures.emplace_back(mDirectory + '/' + texturePath.C_Str(), typeName);
			textureIndices.push_back(mLoadedTextures.size() - 1);
		}
	}
	return textureIndices;
}

Model::Model(const std::string& path, const Transform& transform) : mTransform{ transform } {
	loadModel(path);
	mModel = glm::mat4(1.0);
	mModel = glm::translate(mModel, transform.pos);
	mModel = glm::rotate(mModel, transform.rotation.x, { 1, 0, 0 });
	mModel = glm::rotate(mModel, transform.rotation.y, { 0, 1, 0 });
	mModel = glm::rotate(mModel, transform.rotation.z, { 0, 0, 1 });
	mModel = glm::scale(mModel, transform.scale);
}

const std::vector<Mesh>& Model::getMeshes() const { return mMeshes; }