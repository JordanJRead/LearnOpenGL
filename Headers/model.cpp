#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "structs.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "model.h"
#include <iostream>
#include <glad/glad.h>

struct MeshInfo {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<size_t> textureIndices;
	float shininess;
};

void Model::loadModel(const std::string& path) {
	Assimp::Importer importer{};
	const aiScene* scene{ importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs) };

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << "'\n";
		return;
	}
	mDirectory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
	int x;
	x = 5;
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	for (size_t i{ 0 }; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		MeshInfo meshInfo{ processMesh(mesh, scene) };
		//mMeshes.emplace_back(this, meshInfo.vertices, meshInfo.indices, meshInfo.textureIndices, meshInfo.shininess);
	}

	for (size_t i{ 0 }; i < node->mNumChildren; ++i) {
		processNode(node->mChildren[i], scene);
	}
}

MeshInfo Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<size_t> textureIndices;

	for (size_t vertI{ 0 }; vertI < mesh->mNumVertices; ++vertI) {
		Vertex vertex{};
		glm::vec3 pos{};
		pos.x = mesh->mVertices[vertI].x;
		pos.y = mesh->mVertices[vertI].y;
		pos.z = mesh->mVertices[vertI].z;
		vertex.pos = pos;

		glm::vec3 normal{};
		normal.x = mesh->mNormals[vertI].x;
		normal.y = mesh->mNormals[vertI].y;
		normal.z = mesh->mNormals[vertI].z;
		vertex.normal = normal;

		if (mesh->mTextureCoords[0]) {
			glm::vec2 texCoords{};
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
		std::vector<size_t> diffuseIndices = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::diffuse);
		textureIndices.insert(textureIndices.end(), diffuseIndices.begin(), diffuseIndices.end());
		std::vector<size_t> specularIndices = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::specular);
		textureIndices.insert(textureIndices.end(), specularIndices.begin(), specularIndices.end());
		if (AI_SUCCESS != aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess)) {
			shininess = 32;
		}
	}

	return MeshInfo{ vertices, indices, textureIndices, shininess };
}

std::vector<size_t> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType) {
	std::vector<size_t> textureIndices;
	for (size_t meshTextureIndex{ 0 }; meshTextureIndex < mat->GetTextureCount(type); ++meshTextureIndex) {
		aiString texturePath;
		mat->GetTexture(type, meshTextureIndex, &texturePath);
		bool alreadyLoaded{ false };
		for (size_t loadedTextureIndex{ 0 }; loadedTextureIndex < mTextures.size(); ++loadedTextureIndex) {
			if (std::strcmp(mTextures[loadedTextureIndex].mPath.data(), texturePath.C_Str()) == 0) { // changed to full path
				textureIndices.push_back(loadedTextureIndex);
				alreadyLoaded = true;
				break;
			}
		}
		if (!alreadyLoaded) {
			Texture test{ texturePath.C_Str(), textureType };
			mTextures.emplace_back(texturePath.C_Str(), textureType);
			textureIndices.push_back(mTextures.size() - 1);
		}
	}
	return textureIndices;
}

Model::Model(const std::string& path, const Transform& transform, bool hasBorder) : mHasBorder{ hasBorder } {
	loadModel(path);
	mModelMatrix = glm::mat4(1.0);
	mModelMatrix = glm::translate(mModelMatrix, transform.pos);
	mModelMatrix = glm::rotate(mModelMatrix, transform.rotation.x, { 1, 0, 0 });
	mModelMatrix = glm::rotate(mModelMatrix, transform.rotation.y, { 0, 1, 0 });
	mModelMatrix = glm::rotate(mModelMatrix, transform.rotation.z, { 0, 0, 1 });
	mModelMatrix = glm::scale(mModelMatrix, transform.scale);
}

//void Model::draw(Shader& shader) {
//	shader.setMatrix4("model", model);
//	for (size_t i{ 0 }; i < m_meshes.size(); ++i) {
//		m_meshes[i].draw(shader);
//	}
//}

const std::vector<Mesh>& Model::getMeshes() const { return mMeshes; }