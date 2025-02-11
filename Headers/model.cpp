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
		mMeshes.push_back(processMesh(mesh, scene));
	}

	for (size_t i{ 0 }; i < node->mNumChildren; ++i) {
		processNode(node->mChildren[i], scene);
	}
}
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

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
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::diffuse);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::specular);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		if (AI_SUCCESS != aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess)) {
			shininess = 32;
		}
	}

	return Mesh{ vertices, indices, textures, shininess };
}

unsigned int Model::textureFromFile(std::string_view imagePath) {
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
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else {
		std::cerr << "Failed to load texture data\n";
	}
	return ID;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName) {
	std::vector<Texture> textures;
	for (size_t i{ 0 }; i < mat->GetTextureCount(type); ++i) {
		aiString texturePath;
		mat->GetTexture(type, i, &texturePath);
		bool skip{ false };
		for (const Texture& loadedTexture : mLoadedTextures) {
			if (std::strcmp(loadedTexture.path.data(), texturePath.C_Str()) == 0) {
				textures.push_back(loadedTexture);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture texture;
			texture.id = textureFromFile(mDirectory + '/' + texturePath.C_Str());
			texture.type = typeName;
			texture.path = texturePath.C_Str();
			textures.push_back(texture);
			mLoadedTextures.push_back(texture);
		}
	}
	return textures;
}

Model::Model(const std::string& path, const Transform& transform) {
	loadModel(path);
	mModel = glm::mat4(1.0);
	mModel = glm::translate(mModel, transform.pos);
	mModel = glm::rotate(mModel, transform.rotation.x, { 1, 0, 0 });
	mModel = glm::rotate(mModel, transform.rotation.y, { 0, 1, 0 });
	mModel = glm::rotate(mModel, transform.rotation.z, { 0, 0, 1 });
	mModel = glm::scale(mModel, transform.scale);
}

//void Model::draw(Shader& shader) {
//	shader.setMatrix4("model", model);
//	for (size_t i{ 0 }; i < m_meshes.size(); ++i) {
//		m_meshes[i].draw(shader);
//	}
//}

const std::vector<Mesh>& Model::getMeshes() const { return mMeshes; }