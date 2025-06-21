#ifndef SHOW_NORMALS_SHADER_H
#define SHOW_NORMALS_SHADER_H

#include "shader.h"
#include <string>
#include "../model.h"

class ShowNormalsShader : public Shader {
public:
	ShowNormalsShader(const std::string& vertPath, const std::string& geomPath, const std::string& fragPath)
		: Shader{ vertPath, geomPath, fragPath } { }

	void setUniformModel(const glm::mat4& model) const {
		setMatrix4("model", model);
	}

	void renderModel(const Model& model) {
		setUniformModel(model.mTransform.getModelMatrix());
		for (const Mesh& mesh : model.getMeshes()) {
			glBindVertexArray(mesh.mVAO);
			glDrawElements(GL_TRIANGLES, mesh.mVertexCount, GL_UNSIGNED_INT, nullptr);
		}
	}
};

#endif