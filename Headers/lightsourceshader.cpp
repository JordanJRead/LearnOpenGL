#include "shader.h"
#include <vector>
#include "pointLight.h"
#include "structs.h"
#include "spotLight.h"
#include "camera.h"
#include "lightsourceshader.h"

void LightSourceShader::setUniformModel(const glm::mat4& model) const {
	setMatrix4("model", model);
}
void LightSourceShader::setUniformView(const glm::mat4& view) const {
	setMatrix4("view", view);
}
void LightSourceShader::setUniformProjection(const glm::mat4& projection) const {
	setMatrix4("projection", projection);
}
void LightSourceShader::setUniformLightColor(const glm::vec3& lightColor) const {
	setVector3("lightColor", lightColor);
}

LightSourceShader::LightSourceShader(std::string_view vertPath, std::string_view fragPath)
	: Shader{ vertPath, fragPath }
{
}

void LightSourceShader::render(const Scene& scene, const Camera& camera) {
	use();
	setUniformView(camera.mView);
	setUniformProjection(camera.mProjection); // is once per frame best?

	for (const PointLight& pointLight : scene.getPointLights()) {
		pointLight.modelInfo.use();
		setUniformModel(pointLight.modelInfo.model);
		setUniformLightColor(pointLight.colors.diffuse);
		glDrawArrays(GL_TRIANGLES, 0, pointLight.modelInfo.vertexCount);
	}
	for (const SpotLight& spotLight : scene.getSpotLights()) {
		spotLight.modelInfo.use();
		setUniformModel(spotLight.modelInfo.model);
		setUniformLightColor(spotLight.colors.diffuse);
		glDrawArrays(GL_TRIANGLES, 0, spotLight.modelInfo.vertexCount);
	}
}