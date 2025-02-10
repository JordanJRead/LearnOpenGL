#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "Headers/lightingshader.h"
#include "Headers/lightsourceshader.h"
#include "Headers/texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Headers/camera.h"
#include <vector>
#include "Headers/object.h"
#include "Headers/scene.h"
#include "Headers/structs.h"
#include "Headers/model.h"

float g_deltaTime;
int g_width{ 800 };
int g_height{ 600 };
Camera g_camera{ g_width, g_height, { 0, 0, 3 } };

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    float moveMag{ g_camera.speed * g_deltaTime };
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        g_camera.moveBy(moveMag * g_camera.forward);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        g_camera.moveBy(-moveMag * g_camera.forward);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        g_camera.moveBy(moveMag * glm::normalize(glm::cross(g_camera.forward, g_camera.up)));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        g_camera.moveBy(-moveMag * glm::normalize(glm::cross(g_camera.forward, g_camera.up)));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        g_camera.moveBy(moveMag * g_camera.up);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        g_camera.moveBy(-moveMag * g_camera.up);
    }
}

float sinRange(float x, float y) {
    if (x > y) std::swap(x, y);
    return (y - x) / 2 * sin(glfwGetTime()) + (x+y)/2;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window{ glfwCreateWindow(g_width, g_height, "LearnOpenGL", nullptr, nullptr) };
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window\n";
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos) { g_camera.mouseCallback(window, xPos, yPos); });
    glfwSetScrollCallback(window,    [](GLFWwindow* window, double xPos, double yPos) { g_camera.scrollCallback(window, xPos, yPos); });

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, g_width, g_height);
    glEnable(GL_DEPTH_TEST);
    
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height){ glViewport(0, 0, width, height); });

    std::vector<float> cubeVertices {
         // positions          // normals            // texture coords
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f
    };
    glClearColor(0.2, 0.2, 0.2, 1);

    using Direction = glm::vec3;

    Scene scene{};
    scene.setDirLight(DirLight { Direction {0, -1, 0}, MultiColors {{0.2, 0.2, 0.2}, {1, 1, 1}, {0.5, 0.5, 0.5}} });

    scene.addSpotLight(MultiColors{ { 0.2, 0.2, 0.2 }, { 1, 1, 1 }, { 0.5, 0.5, 0.5 } }, Direction{ 0, -1, 0 }, cos(glm::radians(0.0f)), cos(glm::radians(17.0f)), cubeVertices, Transform{ {0, 5, 0}, {0.2, 0.2, 0.2}, {0, 0, 0} });

    scene.addPointLight(MultiColors{ { 0.2, 0.2, 0.2 }, { 1, 1, 1 }, { 1, 1, 1 } }, Attenuation{ 1, 0.22, 0.2 }, cubeVertices, Transform{ { 0, 0, 0 }, { 0.2, 0.2, 0.2 } });

    Transform transform{ {}, {10, 1, 10}, {0, 0, 0}};
    Material material{ "images/container.png", "images/container_specular.png", "images/emission.jpg", 32 };
    scene.addObject(material, cubeVertices, transform);

    transform = { {0, 10, 0}, {10, 1, 10}, {0, 0, 0} };
    scene.addObject(material, cubeVertices, transform);

    transform = { {0, 5, 5}, {10, 1, 10}, {glm::radians(90.0f), 0, 0} };
    scene.addObject(material, cubeVertices, transform);

    transform = { {0, 5, -5}, {10, 1, 10}, {glm::radians(90.0f), 0, 0} };
    scene.addObject(material, cubeVertices, transform);

    transform = { {5, 5, 0}, {10, 1, 10}, {0, glm::radians(90.0f), glm::radians(90.0f)} };
    scene.addObject(material, cubeVertices, transform);

    transform = { {-5, 5, 0}, {10, 1, 10}, {0, glm::radians(90.0f), glm::radians(90.0f)} };
    scene.addObject(material, cubeVertices, transform);

    // Shaders
    LightingShader lightingShader{ "shaders/lighting.vert", "shaders/lighting.frag" };

    LightSourceShader lightSourceShader{ "shaders/lightSource.vert", "shaders/lightSource.frag" };

    // Delta time and rendering loop
    float currentFrame = glfwGetTime();
    float lastFrame = currentFrame;
    transform = Transform{ {0, 0, 0}, {1, 1, 1}, {0, 0, 0} };
    Model model{ "C:/Users/Jordan/Downloads/backpack/backpack.obj", transform };
    while (!glfwWindowShouldClose(window)) {
        currentFrame = glfwGetTime();
        g_deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window);

        lightingShader.use();
        lightingShader.render(scene, g_camera);
        model.draw(lightingShader);
    
        // Render light source
        lightSourceShader.render(scene, g_camera);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}