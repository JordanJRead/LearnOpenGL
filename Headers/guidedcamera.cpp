#include "guidedcamera.h"
#include "glm/gtc/constants.hpp"
#include <iostream>

void GuidedCamera::givePermission() {
	if (!mIsActive) {
		mHasPermission = true;
	}
}

GuidedCamera::GuidedCamera(int width, int height, const glm::vec3& pos, float fov, float yaw, float pitch)
	: Camera(width, height, pos, fov, yaw, pitch)
{
	setPos({ 0, 5, 0 });

	glm::vec3 slide1View{ -0.86742, 1.5, 7.6187 };
	glm::vec3 slide1Pos{ -0.79, 0.88, 10.95 };

	glm::vec3 slide2View{ -3.9088, 1.5, 5.9268 };
	glm::vec3 slide2Pos{ -6.419, 0.94658, 8.128 };

	glm::vec3 slide3View{ -4.6167, 1.5, 0.8746 };
	glm::vec3 slide3Pos{ -8.0443, 0.94658, -0.93984 };

	glm::vec3 slide4View{ -1.0851, 1.5, -3.999 };
	glm::vec3 slide4Pos{ -4.3403, 1.5, -6.6327 };

	glm::vec3 slide5View{ 1.6049, 1.5, -8.9306 };
	glm::vec3 slide5Pos{ 0.17257, 0.94658, -12.412 };

	glm::vec3 slide6View{ 6.3715, 1.5, -10.002 };
	glm::vec3 slide6Pos{ 7.8623, 0.94658, -13.225 };

	glm::vec3 slide7View{ 7.7583, 1.5, -7.733 };
	glm::vec3 slide7Pos{ 11.06, 1.1335, -7.531 };

	mActions = {
		{Action::look, slide1View},
		{Action::move, slide1View, false},
		{Action::look, slide1Pos, false},

		{Action::look, slide2View },
		{Action::move, slide2View, false},
		{Action::look, slide2Pos, false},

		{Action::look, slide3View },
		{Action::move, slide3View, false},
		{Action::look, slide3Pos, false},

		{Action::look, slide4View },
		{Action::move, slide4View, false},
		{Action::look, slide4Pos, false},

		{Action::look, slide5View },
		{Action::move, slide5View, false},
		{Action::look, slide5Pos, false},

		{Action::look, slide6View },
		{Action::move, slide6View, false},
		{Action::look, slide6Pos, false},

		{Action::look, slide7View },
		{Action::move, slide7View, false},
		{Action::look, slide7Pos, false},
	};
}

double GuidedCamera::smooth(double t) {
	return -(cos(glm::pi<double>() * t) - 1) / 2.0;
}

void GuidedCamera::update() {
	if (mCurrentActionIndex > mActions.size() - 1) {
		return;
	}
	Action& currentAction = mActions[mCurrentActionIndex];
	if (!mIsActive && (mHasPermission || !currentAction.needsPermission)) {
		mIsActive = true;
		mHasPermission = false;
		mActionStartTime = glfwGetTime();
		if (currentAction.type == Action::move) {
			mStartingPos = mPos;
		}
		else if (currentAction.type == Action::look) {
			mStartingPos = { mYaw, 0, 0 };
		}
	}

	if (mIsActive) {
		if (currentAction.type == Action::move) {
			double dist = (mStartingPos - currentAction.position).length();
			double totalTime = dist / mMoveSpeed;
			double t = (glfwGetTime() - mActionStartTime) / totalTime;

			if (t > 1) {
				setPos(currentAction.position);
				mIsActive = false;
				++mCurrentActionIndex;
			}

			float smoothT = smooth(t);
			setPos(mStartingPos + (currentAction.position - mStartingPos) * smoothT);
		}

		else if (currentAction.type == Action::look) {
			double startingYaw = mStartingPos.x;
			glm::vec3 delta = currentAction.position - mPos;
			glm::vec2 destDir = { delta.x, delta.z };
			double destYaw = glm::degrees(atan2(-destDir.y, destDir.x));

			if (destYaw - startingYaw > 180) {
				destYaw -= 360;
			}
			else if (startingYaw - destYaw > 180) {
				destYaw += 360;
			}

			double deltaYaw = destYaw - startingYaw;
			double totalTime = abs(deltaYaw) / mAngleSpeed;
			double t = (glfwGetTime() - mActionStartTime) / totalTime;

			if (t > 1) {
				lookAt(currentAction.position);
				mIsActive = false;
				++mCurrentActionIndex;
			}

			float smoothT = smooth(t);
			double newYaw = startingYaw + deltaYaw * smoothT;
			setYaw(newYaw);
		}
	}
}