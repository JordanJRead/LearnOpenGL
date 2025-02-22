#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"

Camera::Camera(int width, int height, const glm::vec3& pos, float fov, float yaw, float pitch)
    : mScreenWidth{ width }
    , mScreenHeight{ height }
    , mPrevMouseX{ width / 2.0f }
    , mPrevMouseY{ height / 2.0f }
    , mYaw{ yaw }
    , mPitch{ pitch }
    , mFov{ fov }
    , mPos{ pos }
    , mProjection{ glm::perspective(glm::radians(fov), static_cast<float>(width) / height, 0.1f, 100.0f) }
{
    mForward.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    mForward.y = sin(glm::radians(mPitch));
    mForward.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    mView = glm::lookAt(mPos, mPos + mForward, mUp);
}

void Camera::mouseCallback(GLFWwindow* window, double xPos, double yPos) {
    if (mIsFirstMouse) {
        mPrevMouseX = xPos;
        mPrevMouseY = yPos;
        mIsFirstMouse = false;
        return;
    }
    float deltaX = xPos - mPrevMouseX;
    float deltaY = yPos - mPrevMouseY;
    mPrevMouseX = xPos;
    mPrevMouseY = yPos;

    deltaX *= mSensitivity;
    deltaY *= mSensitivity;
    mYaw += deltaX;
    mPitch -= deltaY;

    if (mPitch > 80.0f) mPitch = 80.0f;
    if (mPitch < -80.0f) mPitch = -80.0f;

    glm::vec3 direction{};
    direction.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    direction.y = sin(glm::radians(mPitch));
    direction.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    mForward = glm::normalize(direction);
    mView = glm::lookAt(mPos, mPos + mForward, mUp);
}

void Camera::scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    mFov -= yOffset;
    if (mFov < 1) mFov = 1;
    if (mFov > 45) mFov = 45;
    mProjection = glm::perspective(glm::radians(mFov), 800.0f / 600.0f, 0.1f, 100.0f);
}

void Camera::moveBy(const glm::vec3& v) {
    mPos += v;
    mView = glm::lookAt(mPos, mPos + mForward, mUp);
}