#ifndef INSTANCED_SHADER_H
#define INSTANCED_SHADER_H

#include "shader.h"
#include "../texturetype.h"
#include "../camera.h"
#include "../model.h"
#include "../mesh.h"
#include "../textureutils.h"

class InstancedShader : public Shader {
	friend class Renderer;

protected:

	void setTexture(unsigned int textureIndex, TextureType type) {
		glActiveTexture(GL_TEXTURE0 + (int)type);
		if (type == TextureType::skybox) {
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureIndex);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, textureIndex);
		}
	}

	void renderModel(int dim, const Model& model, const TextureUtils::DefaultTextures& defaultTextures) {
		setInt("dim", dim);
		for (const Mesh& mesh : model.getMeshes()) {
			glBindVertexArray(mesh.mVAO);
			setTexture(mesh.getFirstDiffuseMap(model.mLoadedTextures, defaultTextures), TextureType::diffuse);
			glDrawElementsInstanced(GL_TRIANGLES, mesh.mVertexCount, GL_UNSIGNED_INT, 0, dim * dim);
		}
	}

public:
	InstancedShader(std::string_view vertPath, std::string_view fragPath) : Shader{ vertPath, fragPath } {
		use();
		setInt("diffuseMap", (int)TextureType::diffuse);
	}
};

#endif