#ifndef INSTANCED_SHADER_H
#define INSTANCED_SHADER_H

#include "shader.h"
#include "../camera.h"
#include "../model.h"
#include "../mesh.h"
#include "../textureutils.h"

class InstancedShader : public Shader {
	friend class Renderer;

protected:

	Texture2D perlinNoise{ "images/perlin.png" };
	Texture2D yellowGrass{ "images/grassyellow.png" };

	void setTexture(unsigned int textureIndex, TextureUtils::Type type) {
		glActiveTexture(GL_TEXTURE0 + (int)type);
		if (type == TextureUtils::Type::skybox) {
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureIndex);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, textureIndex);
		}
	}

	void renderModel(int dim, const Model& model, const TextureUtils::DefaultTextures2D& defaultTextures) {
		setInt("dim", dim);
		setFloat("time", glfwGetTime());
;		for (const Mesh& mesh : model.getMeshes()) {
			glBindVertexArray(mesh.mVAO);
			setTexture(mesh.getFirstDiffuseMap(), TextureUtils::Type::diffuse);
			setTexture(perlinNoise.mTex, TextureUtils::Type::emission);
			setTexture(yellowGrass.mTex, TextureUtils::Type::specular);
			glDrawElementsInstanced(GL_TRIANGLES, mesh.mVertexCount, GL_UNSIGNED_INT, 0, dim * dim);
		}
	}

public:
	InstancedShader(std::string_view vertPath, std::string_view fragPath) : Shader{ vertPath, fragPath } {
		use();
		setInt("diffuseMap", (int)TextureUtils::Type::diffuse);
		setInt("perlinNoise", (int)TextureUtils::Type::emission);
		setInt("yellowTex", (int)TextureUtils::Type::specular);

		unsigned int lights_index = glGetUniformBlockIndex(mID, "Matrices");
		glUniformBlockBinding(mID, lights_index, 0);
	}
};

#endif