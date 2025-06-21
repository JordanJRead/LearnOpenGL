#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H
#include <glm/glm.hpp>

class SpotLight {
public:
	SpotLight(const glm::vec3& color, const glm::vec3& direction, float cutoffDot, float outerDot, const glm::vec3& position, float ambientScale = 0.2)
		: mColor{ color }
		, mDirection{ direction }
		, mCutoffDot{ cutoffDot }
		, mOuterDot{ outerDot }
		, mPosition{ position }
		, mAmbientScale{ ambientScale }
	{
	}

	glm::vec3 mPosition{};
	glm::vec3 mDirection{};
	float mCutoffDot{};
	float mOuterDot{};

	glm::vec3 mColor;
	float mAmbientScale{ 0.2 };
};

#endif