#ifndef DEPTH_SHADER_H
#define DEPTH_SHADER_H

#include "shader.h"
#include <string>
#include "../model.h"

class DepthShader : public Shader {
public:
	DepthShader(const std::string& vertPath, const std::string& fragPath) : Shader{ vertPath, fragPath } {}

	void RenderModel(const Model& model) {
		setMatrix4("model", model.mTransform.getModelMatrix());
		for (const Mesh& mesh : model.getMeshes()) {
			glBindVertexArray(mesh.mVAO);
			glDrawElements(GL_TRIANGLES, mesh.mVertexCount, GL_UNSIGNED_INT, 0);
		}
	}
};

#endif