#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>

class Camera {
private:
    bool mIsFirstMouse{ true };
    float mPrevMouseX{ 0 };
    float mPrevMouseY{ 0 };
    float mYaw{ -90 };
    float mPitch{ 0 };
    float mSensitivity{ 0.1 };

    int mScreenWidth{};
    int mScreenHeight{};

    float mFov{ 45 };
    glm::vec3 mForward{};

public:
    glm::vec3 mPos{};
    glm::vec3 mUp{ 0, 1, 0 };

    glm::mat4 mProjection{};
    glm::mat4 mView{};

    float mSpeed{ 2.5 };

    Camera(int width, int height, const glm::vec3& pos = { 0, 0, 0 }, float fov = 45, float yaw = -90, float pitch = 0);
    void mouseCallback(GLFWwindow* window, double xPos, double yPos);
    void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
    void moveBy(const glm::vec3& v);
    void setScreenDimensions(int width, int height);
    void setForward(const glm::vec3& forward);
    const glm::vec3& getForward() const;
};

#endif