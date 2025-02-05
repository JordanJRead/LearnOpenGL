#ifndef STRUCTS_H
#define STRUCTS_H
#include "texture.h"
#include <glm/glm.hpp>
#include <string>

struct Material {
	std::string diffuseMapPath;
	std::string specularMapPath;
	std::string emissionMapPath;
	float shininess;
};

struct Transform {
	glm::vec3 pos;
	glm::vec3 scale;
	glm::vec3 rotation;
};

#endif