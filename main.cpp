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

#define STB_IMAGE_IMPLEMENTATION
#include "Headers/stb_image.h"

class App {
    void processInput(GLFWwindow* window, ScreenQuadShader& effectShader) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            effectShader.startEffect();
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

    static void mouseCallback(GLFWwindow* window, double xPos, double yPos) {
        static_cast<App*>(glfwGetWindowUserPointer(window))->mCamera.mouseCallback(window, xPos, yPos);
    }
    static void scrollCallback(GLFWwindow* window, double xPos, double yPos) {
        static_cast<App*>(glfwGetWindowUserPointer(window))->mCamera.scrollCallback(window, xPos, yPos);
    }
    static void screenResizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        App* appPtr{ static_cast<App*>(glfwGetWindowUserPointer(window)) };
        appPtr->mCamera.setScreenDimensions(width, height);
        appPtr->mScene.updateFramebufferSize(width, height);
        appPtr->mScreenWidth = width;
        appPtr->mScreenHeight = height;
    }

    int mScreenWidth;
    int mScreenHeight;
    Camera mCamera;
    Scene mScene;

    LightingShader mLightingShader;
    LightSourceShader mLightSourceShader;
    BorderShader mBorderShader;
    ScreenQuadShader mScreenQuadShader;
    SkyBoxShader mSkyBoxShader;

    double mCurrentFrame{};
    double mDeltaTime{};
    double mLastFrame{};

public:
    GLFWwindow* mWindow;
    App(int screenWidth, int screenHeight, GLFWwindow* window)
        : mWindow{ window }
        , mScreenWidth{ screenWidth }
        , mScreenHeight{ screenHeight }
        , mCamera{ screenWidth, screenHeight, { 0, 0, 3 } }
        , mScene{ screenWidth, screenHeight, {"images/skybox/blue.png", "images/skybox/green.png", "images/skybox/white.png", "images/skybox/yellow.png", "images/skybox/red.png", "images/skybox/orange.png"}}
        , mLightingShader{ "shaders/lighting.vert", "shaders/lighting.frag" }
        , mLightSourceShader{ "shaders/lightSource.vert", "shaders/lightsource.frag" }
        , mBorderShader{ "shaders/border.vert", "shaders/border.frag" }
        , mScreenQuadShader{ "shaders/screenquad.vert", "shaders/screenquad.frag" }
        , mSkyBoxShader{ "shaders/skybox.vert", "shaders/skybox.frag" }
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

        mScene.setDirLight(DirLight{ Direction {0, -1, 0}, MultiColors {{0.2, 0.2, 0.2}, {1, 1, 1}, {0.5, 0.5, 0.5}} });

        mScene.addSpotLight(MultiColors{ { 0.2, 0.2, 0.2 }, { 0.7, 0, 0 }, { 0.5, 0.5, 0.5 } }, Direction{ 0, -1, 0 }, cos(glm::radians(0.0f)), cos(glm::radians(17.0f)), cubeVertices, Transform{ {0, 7, 0}, {0.2, 0.2, 0.2}, {0, 0, 0} });

        mScene.addPointLight(MultiColors{ { 0.2, 0.2, 0.2 }, { 1, 1, 1 }, { 1, 1, 1 } }, Attenuation{ 1, 0.1, 0.01 }, cubeVertices, Transform{ { 0, 3, 2 }, { 0.2, 0.2, 0.2 } });

        // Delta time and rendering loop
        float currentFrame = glfwGetTime();
        float lastFrame = currentFrame;
        Transform transform = { {0, 0, 0}, {1, 1, 1}, {0, 0, 0} };
        mScene.addModel("Objects/Room/room.obj", transform);
        transform.pos += glm::vec3{ 0, 0, 7 };
        mScene.addTransparentModel("Objects/Window/window.obj", transform);
        transform.pos -= glm::vec3{ 0, 0, 1 };
        mScene.addTransparentModel("Objects/Window/window.obj", transform);
        transform.pos -= glm::vec3{ 0, 0, 1 };
        mScene.addTransparentModel("Objects/Window/window.obj", transform);
        transform = { {3, 3, -3 }, {1, 1, 1}, {0, 0, 0} };
        mScene.addModel("Objects/Backpack/backpack.obj", transform);
        transform = { {-3, 3, -3 }, {1, 1, 1}, {0, 0, 0} };
        mScene.addModel("Objects/Cube/cube.obj", transform);

        transform = { {0, 0, -5}, {2, 2, 2}, {0, 0, 0} };
        //scene.addModel("C:/Users/Jordan/Downloads/backpack/backpack.obj", transform);
    }

    ~App() {
        glfwTerminate();
    }

    void runFrame() {
        mScene.sortTransparent(mCamera.getPos());
        //glfwSwapInterval(0); // show true fps
        //std::cout << 1.0f / g_deltaTime << "\n";
        mCurrentFrame = glfwGetTime();
        mDeltaTime = mCurrentFrame - mLastFrame;
        mLastFrame = mCurrentFrame;

        processInput(mWindow, mScreenQuadShader);

        // Useless if covering whole screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        mLightingShader.render(mScene, mCamera);
        mLightSourceShader.render(mScene, mCamera);
        mSkyBoxShader.render(mScene, mCamera);

        glBindFramebuffer(GL_FRAMEBUFFER, mScene.mFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        mCamera.setForward(mCamera.getForward() * -1.0f);
        mLightingShader.render(mScene, mCamera);
        mSkyBoxShader.render(mScene, mCamera);

        mLightSourceShader.render(mScene, mCamera);
        mCamera.setForward(mCamera.getForward() * -1.0f);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //

        mScreenQuadShader.render(mScene, mCamera);
        

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
};

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

    App app{ 800, 600, window };
    while (!glfwWindowShouldClose(app.mWindow)) {
        app.runFrame();
    }
    return 0;
}