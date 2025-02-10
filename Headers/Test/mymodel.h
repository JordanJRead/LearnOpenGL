#ifndef MY_MODEL_H
#define MY_MODEL_H

#include <string>
#include <assimp/scene.h>
#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>
#include <vector>
#include "mymesh.h"

class MyModel {
	std::vector<MyMesh> meshes;

	unsigned int textureFromFile(std::string_view imagePath) {
		unsigned int ID;
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		int width, height, channelCount;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(imagePath.data(), &width, &height, &channelCount, 0);

		if (data) {
			auto internalFormat{ channelCount == 3 ? GL_RGB : GL_RGBA };
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(data);
		}
		else {
			std::cerr << "Failed to load texture data\n";
		}
		return ID;
	}

	MyMesh processMesh(aiMesh* mesh, const aiScene* scene) {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<MyMeshTexture> textures;

		for (size_t i{ 0 }; i < mesh->mNumVertices; ++i) {
			Vertex vertice;

			vertice.pos.x = mesh->mVertices[i].x;
			vertice.pos.y = mesh->mVertices[i].y;
			vertice.pos.z = mesh->mVertices[i].z;

			vertice.normal.x = mesh->mNormals[i].x;
			vertice.normal.y = mesh->mNormals[i].y;
			vertice.normal.z = mesh->mNormals[i].z;

			vertice.texCoords.x = mesh->mTextureCoords[0][i].x;
			vertice.texCoords.y = mesh->mTextureCoords[0][i].y;

			vertices.push_back(vertice);
		}

		for (size_t i{ 0 }; i < mesh->mNumFaces; ++i) {
			for (size_t j{ 0 }; j < mesh->mFaces[i].mNumIndices; ++i) {
				indices.push_back(mesh->mFaces[i].mIndices[j]);
			}
		}

		aiMaterial* material{ scene->mMaterials[mesh->mMaterialIndex] };
		for (size_t i{ 0 }; i < material->GetTextureCount(aiTextureType_DIFFUSE); ++i) {
			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, i, &path);
			unsigned int id{ textureFromFile(path.C_Str()) };
			textures.push_back(MyMeshTexture{ id, "textire_diffuse", path.C_Str() });
		}

		return MyMesh{ vertices, indices, textures };
	}

	void processNode(aiNode* node, const aiScene* scene) {
		for (size_t i{ 0 }; i < node->mNumMeshes; ++i) {
			meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]]));
		}
		for (size_t i{ 0 }; i < node->mNumChildren; ++i) {
			processNode(node->mChildren[i], scene);
		}
	}

public:
	MyModel(const std::string& filePath) {
		Assimp::Importer importer{};
		const aiScene* scene{ importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs)};

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cerr << "ERROR WHILE LOADING MODEL" << importer.GetErrorString() << "\n";
			return;
		}
		processNode(scene->mRootNode, scene);
	}
};

#endif