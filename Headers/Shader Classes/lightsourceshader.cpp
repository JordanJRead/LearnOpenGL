#include <vector>
#include <glm/glm.hpp>
#include <string_view>
#include "../camera.h"
#include "../scene.h"
#include "lightsourceshader.h"
#include "shader.h"
#include "../pointLight.h"
#include "../spotLight.h"
#include <glad/glad.h>

void LightSourceShader::setUniformModel(const glm::mat4& model) const {
	setMatrix4("model", model);
}
void LightSourceShader::setUniformLightColor(const glm::vec3& lightColor) const {
	setVector3("lightColor", lightColor);
}

LightSourceShader::LightSourceShader(std::string_view vertPath, std::string_view fragPath)
	: Shader{ vertPath, fragPath }
{

	unsigned int lights_index = glGetUniformBlockIndex(mID, "Matrices");
	glUniformBlockBinding(mID, lights_index, 0);
}