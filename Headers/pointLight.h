#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H
#include <glm/glm.hpp>
#include <vector>
#include "structs.h"

class PointLight {
public:
	PointLight(const glm::vec3& color, const Attenuation& attentuation, const glm::vec3& position, float ambientScale = 0.2)
		: mColor{ color }
		, mPosition{ position }
		, mAmbientScale{ ambientScale }
		, mAttenuation{ attentuation }
	{
	}

	glm::vec3 mColor;
	float mAmbientScale{ 0.2 };

	glm::vec3 mPosition{};
	Attenuation mAttenuation;
};

#endif