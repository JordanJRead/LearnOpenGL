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
	void setUniformModel(const glm::mat4& model) const {
		setMatrix4("model", model);
	}
	void setUniformView(const glm::mat4& view) const {
		setMatrix4("view", view);
	}
	void setUniformProjection(const glm::mat4& projection) const {
		setMatrix4("projection", projection);
	}
	void setUniformLightColor(const glm::vec3& lightColor) const {
		setVector3("lightColor", lightColor);
	}

public:
	LightSourceShader(std::string_view vertPath, std::string_view fragPath)
		: Shader{ vertPath, fragPath }
	{
	}

	void render(const Scene& scene, const Camera& camera) override {
		use();
		setUniformView(camera.view);
		setUniformProjection(camera.projection); // is once per frame best?

		for (const PointLight& pointLight : scene.getPointLights()) {
			setUniformModel(pointLight.modelInfo.model);
			setUniformLightColor(pointLight.colors.diffuse);
			glDrawArrays(GL_TRIANGLES, 0, pointLight.modelInfo.vertexCount);
		}
		for (const SpotLight& spotLight : scene.getSpotLights()) {
			setUniformModel(spotLight.modelInfo.model);
			setUniformLightColor(spotLight.colors.diffuse);
			glDrawArrays(GL_TRIANGLES, 0, spotLight.modelInfo.vertexCount);
		}
	}
};
#endif