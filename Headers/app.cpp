#include "app.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader Classes/lightingshader.h"
#include "Shader Classes/lightsourceshader.h"
#include "Shader Classes/bordershader.h"
#include "Shader Classes/screenquadshader.h"
#include "Shader Classes/skyboxshader.h"
#include "camera.h"
#include "scene.h"
#include "structs.h"
#include "stb_image.h"
#include "transform.h"

void App::processInput(GLFWwindow* window, Renderer& renderer) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        renderer.startBlurEffect();
    }
    float moveMag{ mCamera.getSpeed() * (float)mDeltaTime };
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        mCamera.moveBy(moveMag * mCamera.getForward());
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        mCamera.moveBy(-moveMag * mCamera.getForward());
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        mCamera.moveBy(moveMag * glm::normalize(glm::cross(mCamera.getForward(), mCamera.getUp())));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        mCamera.moveBy(-moveMag * glm::normalize(glm::cross(mCamera.getForward(), mCamera.getUp())));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        mCamera.moveBy(moveMag * mCamera.getUp());
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        mCamera.moveBy(-moveMag * mCamera.getUp());
    }
}

void App::mouseCallback(GLFWwindow* window, double xPos, double yPos) {
    static_cast<App*>(glfwGetWindowUserPointer(window))->mCamera.mouseCallback(window, xPos, yPos);
}
void App::scrollCallback(GLFWwindow* window, double xPos, double yPos) {
    static_cast<App*>(glfwGetWindowUserPointer(window))->mCamera.scrollCallback(window, xPos, yPos);
}
void App::screenResizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    App* appPtr{ static_cast<App*>(glfwGetWindowUserPointer(window)) };
    appPtr->mCamera.setScreenDimensions(width, height);
    //appPtr->mScene.updateFramebufferSize(width, height); // todo
    appPtr->mScreenWidth = width;
    appPtr->mScreenHeight = height;
}

App::App(int screenWidth, int screenHeight, GLFWwindow* window)
    : mWindow{ window }
    , mScreenWidth{ screenWidth }
    , mScreenHeight{ screenHeight }
    , mCamera{ screenWidth, screenHeight, { 0, 0, 3 } }
    , mScene{ screenWidth, screenHeight, {"images/skybox/space/px.png", "images/skybox/space/nx.png", "images/skybox/space/py.png", "images/skybox/space/ny.png", "images/skybox/space/pz.png", "images/skybox/space/nz.png"} }
    , mRenderer{ screenWidth, screenHeight, *this }
{
    glfwSetWindowUserPointer(mWindow, this);

    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, double xPos, double yPos) { mouseCallback(window, xPos, yPos); });
    glfwSetScrollCallback(mWindow, [](GLFWwindow* window, double xPos, double yPos) { scrollCallback(window, xPos, yPos); });

    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);

    glfwSetFramebufferSizeCallback(mWindow, screenResizeCallback);

    std::vector<float> cubeVertices{
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
    using Color = glm::vec3;

    mScene.setDirLight(DirLight{ Direction {0, -1, 0}, MultiColors {{0.2, 0.2, 0.2}, {1, 1, 1}, {0.5, 0.5, 0.5}} });

    mScene.addSpotLight(Color{ 0, 1, 0 }, Direction{0, -1, 0}, cos(glm::radians(0.0f)), cos(glm::radians(17.0f)), {0, 7, 0}, 0.2f);
    // fixme
    mScene.addPointLight(Color{ 1, 1, 1 }, Attenuation{ 1, 0.1, 0.01 }, { 0, 3, 2 }, 0.2f);

    // Delta time and rendering loop
    float currentFrame = glfwGetTime();
    float lastFrame = currentFrame;
    Transform transform = { {0, 0, 0}, {1, 1, 1}, {0, 0, 0} };

    mScene.addModel("Objects/Room/room.obj", transform);
    transform.incrementPosition({ 0, 0, 7 });
    mScene.addTransparentModel("Objects/Window/window.obj", transform);
    transform.incrementPosition({ 0, 0, -1 });
    mScene.addTransparentModel("Objects/Window/window.obj", transform);
    transform.incrementPosition({ 0, 0, -1 });
    mScene.addTransparentModel("Objects/Window/window.obj", transform);
    transform = { {3, 3, -3 }, {1, 1, 1}, {0, 0, 0} };
    mScene.addModel("Objects/Backpack/backpack.obj", transform);
    transform = { {-3, 3, -3.5 }, {1, 1, 1}, {0, 0, 0} };
    mScene.addModel("Objects/Cube/cube.obj", transform, false, true);
    transform = { { 0, 3, 0 }, {1, 1, 1}, {0, 0, 0} };
    mScene.addModel("Objects/Sphere/sphere.obj", transform, true);

    transform = { {0, 0, -5}, {2, 2, 2}, {0, 0, 0} };
    mRenderer.createDynamicCubeMaps(mScene, mCamera);
    //mRenderer.createDynamicCubeMaps(mScene, mCamera);
}

void App::runFrame() {
    mScene.sortTransparent(mCamera.getPos());

    glfwSwapInterval(0); // show true fps
    std::cout << 1.0f / mDeltaTime << "\n";

    mCurrentFrame = glfwGetTime();
    mDeltaTime = mCurrentFrame - mLastFrame;
    mLastFrame = mCurrentFrame;

    processInput(mWindow, mRenderer);

    mRenderer.renderScene(mCamera, mScene, true);
    //mRenderer.renderInstanced(mCamera, mScene);
    //mRenderer.renderGeometry();

    glfwSwapBuffers(mWindow);
    glfwPollEvents();
}