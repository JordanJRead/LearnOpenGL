#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Headers/Shader Classes/lightingshader.h"
#include "Headers/Shader Classes/lightsourceshader.h"
#include "Headers/Shader Classes/bordershader.h"
#include "Headers/Shader Classes/screenquadshader.h"
#include "Headers/Shader Classes/skyboxshader.h"
#include "Headers/camera.h"
#include "Headers/scene.h"
#include "Headers/structs.h"
#include "Headers/texture2dmanager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Headers/stb_image.h"

#include "Headers/app.h"
#include <memory>

Texture2DManager* gTexture2DManager;

float sinRange(float x, float y) {
    if (x > y) std::swap(x, y);
    return (y - x) / 2 * sin(glfwGetTime()) + (x+y)/2;
}

int main() {
    int screenWidth{ 800 };
    int screenHeight{ 600 };

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return 1;
    }

    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window\n";
        return 1;
    }

    gTexture2DManager = new Texture2DManager;
    App app{ 800, 600, window };
    while (!glfwWindowShouldClose(app.mWindow)) {
        app.runFrame();
    }
    delete gTexture2DManager;
    return 0;
}