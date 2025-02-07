#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H
#include <glm/glm.hpp>
#include "modelinfo.h"
#include <vector>

class SpotLight {
public:
	// add attenuation?
	glm::vec3 dir{};
	float cutoffDot{};
	float outerDot{};

	MultiColors colors{};

	ModelInfo modelInfo{};
	Transform transform{};
	SpotLight() = default;
	SpotLight(const MultiColors& _colors, const glm::vec3& _dir, float _cutoffDot, float _outerDot, const std::vector<float>& vertices, const Transform& _transform)
		: dir{ _dir }
		, colors{ _colors }
		, cutoffDot{ _cutoffDot }
		, outerDot{ _outerDot }
		, modelInfo{ vertices, _transform }
		, transform{ _transform }
	{
	}
};

#endif