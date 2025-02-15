#ifndef STRUCTS_H
#define STRUCTS_H
#include <glm/glm.hpp>
#include <string>

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

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

struct MultiColors {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct Attenuation {
	float attConst;
	float attLinear;
	float attQuad;
};

struct DirLight {
	glm::vec3 dir;
	MultiColors colors;
};

#endif