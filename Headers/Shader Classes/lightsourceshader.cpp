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

void LightSourceShader::setUniformPosition(const glm::vec3& position) const {
	setVector3("position", position);
}
void LightSourceShader::setUniformLightColor(const glm::vec3& lightColor) const {
	setVector3("lightColor", lightColor);
}

LightSourceShader::LightSourceShader(std::string_view vertPath, std::string_view fragPath)
	: Shader{ vertPath, fragPath }
{
}