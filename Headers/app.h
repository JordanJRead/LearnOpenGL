#pragma once
#include "camera.h"
#include "scene.h"
#include "Shader Classes/lightingshader.h"
#include "Shader Classes/lightsourceshader.h"
#include "Shader Classes/bordershader.h"
#include "Shader Classes/skyboxshader.h"
#include "Shader Classes/screenquadshader.h"
#include "renderer.h"

class GLFWwindow;

class App {
    friend class Renderer;
private:
    void processInput(GLFWwindow* window, Renderer& renderer);
    static void mouseCallback(GLFWwindow* window, double xPos, double yPos);
    static void scrollCallback(GLFWwindow* window, double xPos, double yPos);
    static void screenResizeCallback(GLFWwindow* window, int width, int height);

    int mScreenWidth;
    int mScreenHeight;
    Camera mCamera;
    Scene mScene;
    Renderer mRenderer;

    double mCurrentFrame{};
    double mDeltaTime{};
    double mLastFrame{};

public:
    GLFWwindow* mWindow;
    App(int screenWidth, int screenHeight, GLFWwindow* window);
    void runFrame();
    ~App() {
        glfwTerminate();
    }
};