#ifndef LIGHT_SOURCE_SHADER_H
#define LIGHT_SOURCE_SHADER_H

#include <vector>
#include <glm/glm.hpp>
#include <string_view>
#include "shader.h"

class Scene;
class Camera;

class LightSourceShader : public Shader {
	friend class Renderer;
private:
	void setUniformModel(const glm::mat4& model) const;
	void setUniformLightColor(const glm::vec3& lightColor) const;

public:
	LightSourceShader(std::string_view vertPath, std::string_view fragPath);
};
#endif