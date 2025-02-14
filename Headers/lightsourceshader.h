#ifndef LIGHT_SOURCE_SHADER_H
#define LIGHT_SOURCE_SHADER_H

#include "shader.h"
#include <vector>
#include "pointLight.h"
#include "structs.h"
#include "spotLight.h"
#include "camera.h"

class LightSourceShader : public Shader {
private:
	void setUniformModel(const glm::mat4& model) const;
	void setUniformView(const glm::mat4& view) const;
	void setUniformProjection(const glm::mat4& projection) const;
	void setUniformLightColor(const glm::vec3& lightColor) const;

public:
	LightSourceShader(std::string_view vertPath, std::string_view fragPath);

	void render(const Scene& scene, const Camera& camera) override;
};
#endif