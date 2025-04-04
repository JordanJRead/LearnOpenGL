#include "shader.h"
#include "bordershader.h"
#include <string>
#include <vector>
#include "../pointLight.h"
#include "../spotLight.h"
#include "../model.h"
#include "../camera.h"
#include "../scene.h"
#include "../mesh.h"

void BorderShader::setUniformModel(const glm::mat4& model) const {
	setMatrix4("model", model);
}
void BorderShader::setUniformView(const glm::mat4& view) const {
	setMatrix4("view", view);
}
void BorderShader::setUniformProjection(const glm::mat4& projection) const {
	setMatrix4("projection", projection);
}
BorderShader::BorderShader(const std::string& vertPath, const std::string& fragPath) : Shader{ vertPath, fragPath } {}