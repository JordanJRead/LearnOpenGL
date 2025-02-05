#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:

    bool isFirstMouse{ true };
    int width{};
    int height{};
    float prevMouseX{ 0 };
    float prevMouseY{ 0 };
    float yaw{ -90 };
    float pitch{ 0 };
    float fov{ 45 };
    float sensitivity{ 0.1 };

    glm::vec3 pos{};
    glm::vec3 forward{};
    glm::vec3 up{ 0, 1, 0 };

    glm::mat4 projection{};
    glm::mat4 view{};

    float speed{ 2.5 };

    Camera(int _width, int _height, const glm::vec3& _pos = { 0, 0, 0 }, float _fov = 45, float _yaw = -90, float _pitch = 0)
        : width{ _width }
        , height{ _height }
        , prevMouseX{ _width/2.0f }
        , prevMouseY{ _height/2.0f }
        , yaw{ _yaw }
        , pitch{ _pitch }
        , fov{ _fov }
        , pos{ _pos }
        , projection{ glm::perspective(glm::radians(_fov), static_cast<float>(_width) / _height, 0.1f, 100.0f) }
    {
        forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        forward.y = sin(glm::radians(pitch));
        forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        view = glm::lookAt(pos, pos + forward, up);
    }

    void mouseCallback(GLFWwindow* window, double xPos, double yPos) {
        if (isFirstMouse) {
            prevMouseX = xPos;
            prevMouseY = yPos;
            isFirstMouse = false;
            return;
        }
        float deltaX = xPos - prevMouseX;
        float deltaY = yPos - prevMouseY;
        prevMouseX = xPos;
        prevMouseY = yPos;

        deltaX *= sensitivity;
        deltaY *= sensitivity;
        yaw += deltaX;
        pitch -= deltaY;

        if (pitch > 80.0f) pitch = 80.0f;
        if (pitch < -80.0f) pitch = -80.0f;

        glm::vec3 direction{};
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        forward = glm::normalize(direction);
        view = glm::lookAt(pos, pos + forward, up);
    }

    void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
        fov -= yOffset;
        if (fov < 1) fov = 1;
        if (fov > 45) fov = 45;
        projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
    }

    void moveBy(const glm::vec3& v) {
        pos += v;
        view = glm::lookAt(pos, pos + forward, up);
    }
};

#endif