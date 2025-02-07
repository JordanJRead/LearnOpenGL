#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H
#include <glm/glm.hpp>
#include "modelinfo.h"
#include <vector>
/*
struct SpotLight {
	vec3 dir;
	float cutoffDot;
	float outerDot;
};
*/
class SpotLight {
public:
	// add attenuation and colors?
	glm::vec3 dir{};
	float cutoffDot{};
	float outerDot{};

	glm::vec3 ambient{};
	glm::vec3 diffuse{};
	glm::vec3 specular{};

	ModelInfo modelInfo{};
	Transform transform{};
	SpotLight() = default;
	SpotLight(const glm::vec3& _dir, float _cutoffDot, float _outerDot, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, const std::vector<float>& vertices, const Transform& _transform)
		: dir{ _dir }
		, ambient{ _ambient }
		, diffuse{ _diffuse }
		, specular{ _specular }
		, cutoffDot{ _cutoffDot }
		, outerDot{ _outerDot }
		, modelInfo{ vertices, _transform }
		, transform{ _transform }
	{
	}
};

#endif