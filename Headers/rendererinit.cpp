#include "renderer.h"
#include <vector>
#include <glm/glm.hpp>

void Renderer::initCubeVertices() {
    std::vector<float> cubeVertices{
        // positions
       -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,

        0.5f,  0.5f, -0.5f,
       -0.5f,  0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,

        0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

       -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
       -0.5f, -0.5f,  0.5f,

       -0.5f,  0.5f, -0.5f,
       -0.5f,  0.5f,  0.5f,
       -0.5f, -0.5f, -0.5f,

       -0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f, -0.5f,
       -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,

        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        0.5f, -0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,

       -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f, -0.5f,

       -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
       -0.5f,  0.5f,  0.5f,
       -0.5f,  0.5f, -0.5f
    };
    glBindVertexArray(mCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(float), &(cubeVertices.data()[0]), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::initScreenQuad() {
    glBindVertexArray(mScreenQuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mScreenQuadVBO);

    float vertices[]{
        -1, -1, 0,  0, 0,
         1, -1, 0,  1, 0,
        -1,  1, 0,  0, 1,

         1,  1, 0,  1, 1,
        -1,  1, 0,  0, 1,
         1, -1, 0,  1, 0
    };

    glBufferData(GL_ARRAY_BUFFER, std::size(vertices) * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
