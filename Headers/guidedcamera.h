#ifndef GUIDED_CAMERA_H
#define GUIDED_CAMERA_H

#include "camera.h"
#include "glm/glm.hpp"
#include <vector>

struct Action {
	enum Type {
		move,
		look
	};

	Type type;
	glm::vec3 position;
	bool needsPermission = true;
};

class GuidedCamera : public Camera {
private:
	bool mHasPermission{ false };
	bool mIsActive{ false };
	double mActionStartTime{};
	std::vector<Action> mActions;
	int mCurrentActionIndex{ 0 };
	double mMoveSpeed{ 5 };
	//double mMoveSpeed{ 10 };
	double mAngleSpeed{ 120 };
	//double mAngleSpeed{ 360 };
	glm::vec3 mStartingPos;

	static double smooth(double t);

public:
	void givePermission();
	GuidedCamera(int width, int height, const glm::vec3& pos = { 0, 0, 0 }, float fov = 45, float yaw = 90, float pitch = 0);
	void update();
};

#endif