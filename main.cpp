#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Headers/Shader Classes/lightingshader.h"
#include "Headers/Shader Classes/lightsourceshader.h"
#include "Headers/Shader Classes/bordershader.h"
#include "Headers/Shader Classes/screenquadshader.h"
#include "Headers/camera.h"
#include "Headers/scene.h"
#include "Headers/structs.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Headers/stb_image.h"

float gDeltaTime;
int gWidth{ 800 };
int gHeight{ 600 };
Camera gCamera{ gWidth, gHeight, { 0, 0, 3 } };

void processInput(GLFWwindow* window, ScreenQuadShader& effectShader) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        effectShader.startEffect();
    }
    float moveMag{ gCamera.mSpeed * gDeltaTime };
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        gCamera.moveBy(moveMag * gCamera.getForward());
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        gCamera.moveBy(-moveMag * gCamera.getForward());
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        gCamera.moveBy(moveMag * glm::normalize(glm::cross(gCamera.getForward(), gCamera.mUp)));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        gCamera.moveBy(-moveMag * glm::normalize(glm::cross(gCamera.getForward(), gCamera.mUp)));
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

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

    GLFWwindow* window{ glfwCreateWindow(gWidth, gHeight, "LearnOpenGL", nullptr, nullptr) };
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

    glViewport(0, 0, gWidth, gHeight);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height){
        glViewport(0, 0, width, height);
        gCamera.setScreenDimensions(width, height);
        gWidth = width;
        gHeight = height;
    });

    std::vector<float> cubeVertices {
         // positions          // normals            // texture coords
         0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
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

         0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,

         0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,

         0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,

         0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f
    };
    glClearColor(0.2, 0.2, 0.2, 1);

    using Direction = glm::vec3;

    Scene scene{ gWidth, gHeight };
    scene.setDirLight(DirLight { Direction {0, -1, 0}, MultiColors {{0.2, 0.2, 0.2}, {1, 1, 1}, {0.5, 0.5, 0.5}} });

    scene.addSpotLight(MultiColors{ { 0.2, 0.2, 0.2 }, { 0.7, 0, 0 }, { 0.5, 0.5, 0.5 } }, Direction{ 0, -1, 0 }, cos(glm::radians(0.0f)), cos(glm::radians(17.0f)), cubeVertices, Transform{ {0, 7, 0}, {0.2, 0.2, 0.2}, {0, 0, 0} });

    scene.addPointLight(MultiColors{ { 0.2, 0.2, 0.2 }, { 1, 1, 1 }, { 1, 1, 1 } }, Attenuation{ 1, 0.1, 0.01 }, cubeVertices, Transform{ { 0, 3, 2 }, { 0.2, 0.2, 0.2 } });

    // Shaders
    LightingShader lightingShader{ "shaders/lighting.vert", "shaders/lighting.frag" };
    LightSourceShader lightSourceShader{ "shaders/lightSource.vert", "shaders/lightSource.frag" };
    BorderShader borderShader{ "shaders/border.vert", "shaders/border.frag" };
    ScreenQuadShader screenQuadShader{ "shaders/screenquad.vert", "shaders/screenquad.frag" };

    // Delta time and rendering loop
    float currentFrame = glfwGetTime();
    float lastFrame = currentFrame;
    Transform transform = { {0, 0, 0}, {1, 1, 1}, {0, 0, 0} };
    scene.addModel("Objects/Room/room.obj", transform);
    transform.pos += glm::vec3{ 0, 0, 7 };
    scene.addTransparentModel("Objects/Window/window.obj", transform);
    transform.pos -= glm::vec3{ 0, 0, 1 };
    scene.addTransparentModel("Objects/Window/window.obj", transform);
    transform.pos -= glm::vec3{ 0, 0, 1 };
    scene.addTransparentModel("Objects/Window/window.obj", transform);
    transform = { {3, 3, -3 }, {1, 1, 1}, {0, 0, 0} };
    scene.addModel("Objects/Backpack/backpack.obj", transform);
    transform = { {-3, 3, -3 }, {1, 1, 1}, {0, 0, 0} };
    scene.addModel("Objects/Cube/cube.obj", transform);

    transform = { {0, 0, -5}, {2, 2, 2}, {0, 0, 0} };
    //scene.addModel("C:/Users/Jordan/Downloads/backpack/backpack.obj", transform);
    while (!glfwWindowShouldClose(window)) {
        scene.sortTransparent(gCamera.mPos);
        //glfwSwapInterval(0); // show true fps
        //std::cout << 1.0f / g_deltaTime << "\n";
        currentFrame = glfwGetTime();
        gDeltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        scene.updateFramebufferSize(gWidth, gHeight);

        processInput(window, screenQuadShader);
        scene.updateFramebufferSize(gWidth, gHeight);

        // Useless if covering whole screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        lightingShader.render(scene, gCamera);
        lightSourceShader.render(scene, gCamera);

        glBindFramebuffer(GL_FRAMEBUFFER, scene.mFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        gCamera.setForward(gCamera.getForward() * -1.0f);
        lightingShader.render(scene, gCamera);

        lightSourceShader.render(scene, gCamera);
        gCamera.setForward(gCamera.getForward() * -1.0f);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //

        screenQuadShader.render(scene, gCamera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}