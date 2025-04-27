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
	mActions = {
		{Action::look, {-1, 0, 0} },
		{Action ::move, {0, 10, 0,} }
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

	if (!mIsActive && mHasPermission) {
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
			double destYaw = glm::degrees(atan2(destDir.y, destDir.x));

			if (destYaw - startingYaw > 180) {
				destYaw -= 360;
			}
			else if (startingYaw - destYaw > 180) {
				destYaw += 360;
			}

			double deltaYaw = destYaw - startingYaw;
			double totalTime = deltaYaw / mAngleSpeed;
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