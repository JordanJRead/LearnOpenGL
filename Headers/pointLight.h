#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H
#include <glm/glm.hpp>
#include "modelinfo.h"
#include <vector>
#include "structs.h"

class PointLight {
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float attConst;
	float attLinear;
	float attQuad;

	ModelInfo modelInfo;

	Transform transform;

	PointLight(const glm::vec3& _ambient, const glm::vec3& _diffuse, const glm::vec3& _specular,
		float _attConst, float _attLinear, float _attQuad,
		const std::vector<float>& vertices, const Transform& _transform)
		: ambient{ _ambient }
		, diffuse{ _diffuse }
		, specular{ _specular }
		, attConst{ _attConst }
		, attLinear{ _attLinear }
		, attQuad{ _attQuad }
		, modelInfo{ vertices, _transform }
		, transform{ _transform }
	{}
};

#endif