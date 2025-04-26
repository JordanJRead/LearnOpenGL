#ifndef GUIDED_CAMERA_H
#define GUIDED_CAMERA_H

#include "camera.h"
#include "glm/glm.hpp"

class GuidedCamera : public Camera {
private:
	bool mHasPermission{ false };
	bool mIsActive{ false };
	float mDesiredYaw{ 0 };
	float mDesiredPitch{ 0 };
	glm::vec3 mDesiredPosition;

};

#endif