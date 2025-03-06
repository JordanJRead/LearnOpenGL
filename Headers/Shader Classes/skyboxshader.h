#pragma once

#include "shader.h"
#include "../OpenGL Wrappers/VAO.h"
#include "../OpenGL Wrappers/VBO.h"

class Scene;
class Camera;

class SkyBoxShader : public Shader {
private:
	VAO mCubeVAO;
	VBO mCubeVBO;
	void setUniformView(const glm::mat4& view) const;
	void setUniformProjection(const glm::mat4& projection) const;
	void setUniformSkyBoxTexture(int unit) const;

public:
	SkyBoxShader(std::string_view vertPath, std::string_view fragPath);

	void render(const Scene& scene, const Camera& camera) override;
};