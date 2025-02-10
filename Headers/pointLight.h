#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H
#include <glm/glm.hpp>
#include <vector>
#include "structs.h"
#include "modelinfo.h"

class PointLight {
public:
	MultiColors colors{};

	Attenuation attenuation;

	ModelInfo modelInfo;

	Transform transform;

	PointLight(const MultiColors& _colors, const Attenuation& _attenuation, const std::vector<float>& vertices, const Transform& _transform)
		: colors{ _colors }
		, attenuation{ _attenuation }
		, modelInfo{ vertices, _transform }
		, transform{ _transform }
	{}
};

#endif