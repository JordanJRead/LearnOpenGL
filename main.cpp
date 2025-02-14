#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Headers/lightingshader.h"
#include "Headers/lightsourceshader.h"
#include "Headers/bordershader.h"
#include "Headers/camera.h"
#include "Headers/scene.h"
#include "Headers/textureshader.h"

float g_deltaTime;
int g_width{ 800 };
int g_height{ 600 };
Camera gCamera{ g_width, g_height, { 0, 0, 3 } };

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    float moveMag{ gCamera.mSpeed * g_deltaTime };
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        gCamera.moveBy(moveMag * gCamera.mForward);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        gCamera.moveBy(-moveMag * gCamera.mForward);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        gCamera.moveBy(moveMag * glm::normalize(glm::cross(gCamera.mForward, gCamera.mUp)));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        gCamera.moveBy(-moveMag * glm::normalize(glm::cross(gCamera.mForward, gCamera.mUp)));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        gCamera.moveBy(moveMag * gCamera.mUp);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        gCamera.moveBy(-moveMag * gCamera.mUp);
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
    glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

    GLFWwindow* window{ glfwCreateWindow(g_width, g_height, "LearnOpenGL", nullptr, nullptr) };
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window\n";
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos) { gCamera.mouseCallback(window, xPos, yPos); });
    glfwSetScrollCallback(window,    [](GLFWwindow* window, double xPos, double yPos) { gCamera.scrollCallback(window, xPos, yPos); });

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glViewport(0, 0, g_width, g_height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    
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

    scene.addPointLight(MultiColors{ { 0.2, 0.2, 0.2 }, { 1, 1, 1 }, { 1, 1, 1 } }, Attenuation{ 1, 0.02, 0.02 }, cubeVertices, Transform{ { 0, 3, 2 }, { 0.2, 0.2, 0.2 } });

    // Shaders
    LightingShader lightingShader{ "shaders/lighting.vert", "shaders/lighting.frag" };
    LightSourceShader lightSourceShader{ "shaders/lightSource.vert", "shaders/lightSource.frag" };
    BorderShader borderShader{ "shaders/border.vert", "shaders/border.frag" };
    TextureShader textureShader{ "shaders/texture.vert", "shaders/texture.frag" };

    // Delta time and rendering loop
    float currentFrame = glfwGetTime();
    float lastFrame = currentFrame;
    Transform transform = { {0, 0, 0}, {1, 1, 1}, {0, 0, 0} };

    transform = { {0, 8, -4}, {2, 2, 2}, {0, 0, 0} };
    scene.addModel("C:/Users/Jordan/Downloads/backpack/backpack.obj", transform);
    scene.addGrassPosition({ 0, 4, 3});
    scene.addGrassPosition({ 0, 4, 4 });
    scene.addGrassPosition({ 1, 4, 5 });

    transform = { {0, 0, 0}, {10, 1, 10}, {0, 0, 0} };
    scene.addModel("C:/Users/Jordan/Desktop/Cube/cube.obj", transform);

    transform = { {0, 20, 0}, {10, 1, 10}, {0, 0, 0} };
    scene.addModel("C:/Users/Jordan/Desktop/Cube/cube.obj", transform);

    transform = { {0, 10, 10}, {10, 1, 10}, {glm::radians(90.0f), 0, 0} };
    scene.addModel("C:/Users/Jordan/Desktop/Cube/cube.obj", transform);

    transform = { {0, 10, -10}, {10, 1, 10}, {glm::radians(90.0f), 0, 0} };
    scene.addModel("C:/Users/Jordan/Desktop/Cube/cube.obj", transform);

    transform = { {10, 10, 0}, {10, 1, 10}, {0, 0, glm::radians(90.0f)} };
    scene.addModel("C:/Users/Jordan/Desktop/Cube/cube.obj", transform, true);

    transform = { {-10, 10, 0}, {10, 1, 10}, {0, 0, glm::radians(90.0f)} };
    scene.addModel("C:/Users/Jordan/Desktop/Cube/cube.obj", transform, true);

    while (!glfwWindowShouldClose(window)) {
        scene.sortTransparent(gCamera.mPos);
        //glfwSwapInterval(0); // show true fps
        //std::cout << 1.0f / g_deltaTime << "\n";
        currentFrame = glfwGetTime();
        g_deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        processInput(window);

        /*
        glBindFramebuffer(GL_FRAMEBUFFER, scene.FBO);
        glClearColor(0.1, 1, 0.1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        lightingShader.render(scene, gCamera);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1, 0.1, 0.1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        //borderShader.render(scene, gCamera);

        textureShader.render(scene, gCamera);
        */

        lightingShader.render(scene, gCamera);
        lightSourceShader.render(scene, gCamera);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}