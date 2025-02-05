#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H
#include <glm/glm.hpp>
/*
struct PointLight {
	vec3 pos;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float attConst;
	float attLinear;
	float attQuad;
};
*/
struct PointLight {
	glm::vec3 pos;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float attConst;
	float attLinear;
	float attQuad;
	// TODO add VertexArrayWrapper?
};

#endif