#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "structs.h"

class Model {
private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<MeshTexture> loadedTextures;

	void loadModel(const std::string& path) {
		Assimp::Importer importer{};
		const aiScene* scene{ importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs) };

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << "'\n";
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
		int x;
		x = 5;
	}
	void processNode(aiNode* node, const aiScene* scene) {
		for (size_t i{ 0 }; i < node->mNumMeshes; ++i) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		for (size_t i{ 0 }; i < node->mNumChildren; ++i) {
			std::cerr << "child #" << i << "\n";
			processNode(node->mChildren[i], scene);
		}
	}
	Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<MeshTexture> textures;

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

		if (mesh->mMaterialIndex >= 0) { // ?
			aiMaterial* material{ scene->mMaterials[mesh->mMaterialIndex] };
			std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			std::vector<MeshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return Mesh{ vertices, indices, textures };
	}

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

	std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
		std::vector<MeshTexture> textures;
		for (size_t i{ 0 }; i < mat->GetTextureCount(type); ++i) {
			aiString texturePath;
			mat->GetTexture(type, i, &texturePath);
			bool skip{ false };
			for (const MeshTexture& loadedTexture : loadedTextures) {
				if (std::strcmp(loadedTexture.path.data(), texturePath.C_Str()) == 0) {
					textures.push_back(loadedTexture);
					skip = true;
					break;
				}
			}
			if (!skip) {
				MeshTexture texture;
				texture.id = textureFromFile(directory + '/' + texturePath.C_Str());
				texture.type = typeName;
				texture.path = texturePath.C_Str();
				textures.push_back(texture);
				loadedTextures.push_back(texture);
			}
		}
		return textures;
	}

public:
	glm::mat4 model;
	Model(const std::string& path, const Transform& transform) {
		loadModel(path);
		model = glm::mat4(1.0);
		model = glm::translate(model, transform.pos);
		model = glm::rotate(model, transform.rotation.x, { 1, 0, 0 });
		model = glm::rotate(model, transform.rotation.y, { 0, 1, 0 });
		model = glm::rotate(model, transform.rotation.z, { 0, 0, 1 });
		model = glm::scale(model, transform.scale);
	}
	void draw(Shader& shader) {
		shader.setMatrix4("model", model);
		for (size_t i{ 0 }; i < meshes.size(); ++i) {
			meshes[i].draw(shader);
		}
	}
};

#endif