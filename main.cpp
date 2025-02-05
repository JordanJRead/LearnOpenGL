#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "Headers/lightingshader.h"
#include "Headers/texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Headers/camera.h"
#include <vector>
#include "Headers/object.h"

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
        //g_camera.moveBy(moveMag * glm::vec3(cos(glm::radians(g_camera.yaw)), 0, sin(glm::radians(g_camera.yaw))));
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

float sinRange(float min, float max) {
    return (max - min) / 2 * sin(glfwGetTime()) + (min+max)/2;
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

    Transform transform{ {}, {10, 1, 10}, {0, 0, 0}}; // rotation fixme
    Material material{ "images/container.png", "images/container_specular.png", "images/emission.jpg", 32 };
    Object cube{ material, cubeVertices, transform };

    // Light source data
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cube.VBO);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glm::vec3 lightPos{ 0, 1, 0 };
    // Shaders
    LightingShader lightingShader{ "shaders/lighting.vert", "shaders/lighting.frag" };
    lightingShader.use();
    lightingShader.setUniformViewPos(g_camera.pos);

    // Object
    lightingShader.setUniformMaterialShininess(32); // ? per object maybe?

    // Light XXX FINISH LIGHT ARRAYS
    //lightingShader.setVector3("light.pos", lightPos);
    //lightingShader.setVector3("light.ambient", 0.2, 0.2, 0.2);
    //lightingShader.setVector3("light.diffuse", 1, 1, 1);
    //lightingShader.setVector3("light.specular", 1, 1, 1);

    //Shader gourandShader{ "shaders/gourand.vert", "shaders/gourand.frag" };
    //gourandShader.use();
    //gourandShader.setVector3("lightColor", 1, 1, 1);
    //gourandShader.setVector3("lightPos", lightPos);
    //gourandShader.setVector3("viewPos", g_camera.pos);

    // XX MAKE LIGHT SOURCE SHADER CLASS
    Shader lightSourceShader{ "shaders/lightSource.vert", "shaders/lightSource.frag" };

    // Matrices
    glm::mat4 lightModel = glm::mat4(1);
    lightModel = glm::translate(lightModel, lightPos);
    lightModel = glm::scale(lightModel, glm::vec3{ 0.2 });

    // Delta time and rendering loop
    float currentFrame = glfwGetTime();
    float lastFrame = currentFrame;
    while (!glfwWindowShouldClose(window)) {

        lightModel = glm::mat4(1);
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3{ 0.2 });

        currentFrame = glfwGetTime();
        g_deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window);

        lightingShader.use();

        // XXX Light arrays
        lightingShader.setFloat("light.attLinear", sinRange(0.0014, 0.7));
        lightingShader.setFloat("light.attLinear", 0.14);
        lightingShader.setFloat("light.attQuad", sinRange(0.000007, 1.8));
        lightingShader.setFloat("light.attQuad", 0.07);

        // TODO add spotlights to shader program / shader object
        lightingShader.setVector3("flashLightDir", g_camera.forward);
        lightingShader.setFloat("cutoffDot", cos(glm::radians(10.5f)));
        lightingShader.setFloat("outerDot", cos(glm::radians(15.0f)));

        lightingShader.setUniformView(g_camera.view);
        lightingShader.setUniformProjection(g_camera.projection);

        // Render cube
        lightingShader.setVector3("light.pos", lightPos);
        lightingShader.setUniformViewPos(g_camera.pos);

        cube.load(lightingShader);
        glDrawArrays(GL_TRIANGLES, 0, cube.vertexCount);

        cube.updateTransform({ {0, 10, 0}, {10, 1, 10}, {0, 0, 0} });
        cube.load(lightingShader);
        glDrawArrays(GL_TRIANGLES, 0, cube.vertexCount);

        cube.updateTransform({ {0, 5, 5}, {10, 1, 10}, {glm::radians(90.0f), 0, 0}});
        cube.load(lightingShader);
        glDrawArrays(GL_TRIANGLES, 0, cube.vertexCount);

        cube.updateTransform({ {0, 5, -5}, {10, 1, 10}, {glm::radians(90.0f), 0, 0} });
        cube.load(lightingShader);
        glDrawArrays(GL_TRIANGLES, 0, cube.vertexCount);

        cube.updateTransform({ {5, 5, 0}, {10, 1, 10}, {0, glm::radians(90.0f), glm::radians(90.0f)} });
        cube.load(lightingShader);
        glDrawArrays(GL_TRIANGLES, 0, cube.vertexCount);

        cube.updateTransform({ {-5, 5, 0}, {10, 1, 10}, {0, glm::radians(90.0f), glm::radians(90.0f)} });
        cube.load(lightingShader);
        glDrawArrays(GL_TRIANGLES, 0, cube.vertexCount);

        cube.updateTransform(transform);
        cube.load(lightingShader);

        // Render light source
        lightSourceShader.use();
        glUniformMatrix4fv(glGetUniformLocation(lightSourceShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
        glUniformMatrix4fv(glGetUniformLocation(lightSourceShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(g_camera.view));
        glUniformMatrix4fv(glGetUniformLocation(lightSourceShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(g_camera.projection));
        lightSourceShader.setVector3("lightColor", 1, 1, 1);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &lightVAO);
    glfwTerminate();
    return 0;
}