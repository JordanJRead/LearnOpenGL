#include "renderer.h"
#include <vector>

void Renderer::initDynamicEnvironment() {
    glBindRenderbuffer(GL_RENDERBUFFER, mDynamicCubeMapDepthStencilRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 1024);

    glActiveTexture(GL_TEXTURE0);
    glBindFramebuffer(GL_FRAMEBUFFER, mDynamicCubeMapFBO);
    glBindTexture(GL_TEXTURE_2D, mDynamicCubeMapColorTex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mDynamicCubeMapColorTex, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDynamicCubeMapDepthStencilRBO);
}

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

void Renderer::initMatricesBuffer() {
    glBindBuffer(GL_UNIFORM_BUFFER, mMatricesBuffer);
    glBufferData(GL_UNIFORM_BUFFER, 128, nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, mMatricesBuffer);
}