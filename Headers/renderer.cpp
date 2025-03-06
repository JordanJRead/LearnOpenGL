#include "renderer.h"
#include "app.h"

void Renderer::renderLightingModel(const Model& model) {
    mLightingShader.setUniformModel(model.mModel);
    for (const Mesh& mesh : model.getMeshes()) {
        mLightingShader.setUniformMaterialShininess(mesh.mShininess);
        glBindVertexArray(mesh.mVAO);

        glActiveTexture(GL_TEXTURE0 + mLightingShader.mDiffuseTextureIndex);
        unsigned int diffuseMap{ mesh.getFirstDiffuseMap(model.mLoadedTextures, mDefaultTextures) };
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        glActiveTexture(GL_TEXTURE0 + mLightingShader.mSpecularTextureIndex);
        unsigned int specularMap{ mesh.getFirstSpecularMap(model.mLoadedTextures, mDefaultTextures) };
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glActiveTexture(GL_TEXTURE0 + mLightingShader.mEmissionTextureIndex);
        unsigned int demissionMap{ mesh.getFirstEmissionMap(model.mLoadedTextures, mDefaultTextures) };
        glBindTexture(GL_TEXTURE_2D, demissionMap);

        glActiveTexture(GL_TEXTURE0 + mLightingShader.mReflectionTextureIndex);
        unsigned int reflectionMap{ mesh.getFirstReflectionMap(model.mLoadedTextures, mDefaultTextures) };
        glBindTexture(GL_TEXTURE_2D, reflectionMap);

        glDrawElements(GL_TRIANGLES, mesh.mVertexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void Renderer::renderLighting(App& app) {
    mLightingShader.use();
    glActiveTexture(GL_TEXTURE0 + mLightingShader.mSkyboxTextureIndex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, app.mScene.getCubeMap().mTEX);
    mLightingShader.setUniformView(app.mCamera.getView());
    mLightingShader.setUniformProjection(app.mCamera.getProjection()); // is once per frame best?
    mLightingShader.setUniformViewPos(app.mCamera.getPos());
    mLightingShader.setUniformViewDir(app.mCamera.getForward());

    mLightingShader.setUniformPointLights(app.mScene.getPointLights());
    mLightingShader.setUniformMaxPointLights(app.mScene.getPointLights().size());
    mLightingShader.setUniformDirLight(app.mScene.getDirLight());
    mLightingShader.setUniformSpotLights(app.mScene.getSpotLights());
    mLightingShader.setUniformMaxSpotLights(app.mScene.getSpotLights().size());
    for (const Model& model : app.mScene.getModels()) {
        renderLightingModel(model);
    }

    for (const Model& model : app.mScene.getTransparentModels()) {
        renderLightingModel(model);
    }

    glBindVertexArray(0);
}

void Renderer::renderLightSources(App& app) {
    mLightSourceShader.use();
    mLightSourceShader.setUniformView(app.mCamera.getView());
    mLightSourceShader.setUniformProjection(app.mCamera.getProjection()); // is once per frame best?

    for (const PointLight& pointLight : app.mScene.getPointLights()) {
        pointLight.modelInfo.use();
        mLightSourceShader.setUniformModel(pointLight.modelInfo.model);
        mLightSourceShader.setUniformLightColor(pointLight.colors.diffuse);
        glDrawArrays(GL_TRIANGLES, 0, pointLight.modelInfo.vertexCount);
    }
    for (const SpotLight& spotLight : app.mScene.getSpotLights()) {
        spotLight.modelInfo.use();
        mLightSourceShader.setUniformModel(spotLight.modelInfo.model);
        mLightSourceShader.setUniformLightColor(spotLight.colors.diffuse);
        glDrawArrays(GL_TRIANGLES, 0, spotLight.modelInfo.vertexCount);
    }
}

void Renderer::renderSkyBox(App& app) {
    mSkyBoxShader.use();
    glDisable(GL_CULL_FACE); // todo
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(mCubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, app.mScene.getCubeMap().mTEX);
    mSkyBoxShader.setUniformView(glm::mat4(glm::mat3(app.mCamera.getView())));
    mSkyBoxShader.setUniformProjection(app.mCamera.getProjection());
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Renderer::renderScreenQuad(App& app, unsigned int texID, bool rearView) {
    mScreenQuadShader.use();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE); // todo

    mScreenQuadShader.setUniformOffset(mScreenQuadShader.getOffset(mScreenQuadShader.mEffectStartTime));
    if (rearView) {
        mScreenQuadShader.setUniformModel(mRearViewMatrix);
    }
    else {
        mScreenQuadShader.setUniformModel({1});
    }

    glBindVertexArray(mScreenQuadVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::initReverseFBO(int screenWidth, int screenHeight) {
    glBindFramebuffer(GL_FRAMEBUFFER, mReverseFBO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mReverseColorTex);
    glBindRenderbuffer(GL_RENDERBUFFER, mReverseDepthStencilRBO);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mReverseColorTex, 0);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mReverseDepthStencilRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::initCubeVertices() {
    std::vector<float> cubeVertices{
        // positions
        0.5f, -0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,

       -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,

       -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
       -0.5f,  0.5f,  0.5f,
       -0.5f, -0.5f,  0.5f,

       -0.5f,  0.5f,  0.5f,
       -0.5f,  0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,

       -0.5f, -0.5f, -0.5f,
       -0.5f, -0.5f,  0.5f,
       -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f, -0.5f, -0.5f,

        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,

       -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,

        0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f, -0.5f,

        0.5f,  0.5f, -0.5f,
       -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,

       -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
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

Renderer::Renderer(int screenWidth, int screenHeight)
    : mLightingShader{ "shaders/lighting.vert", "shaders/lighting.frag" }
    , mLightSourceShader{ "shaders/lightSource.vert", "shaders/lightsource.frag" }
    , mBorderShader{ "shaders/border.vert", "shaders/border.frag" }
    , mScreenQuadShader{ "shaders/screenquad.vert", "shaders/screenquad.frag" }
    , mSkyBoxShader{ "shaders/skybox.vert", "shaders/skybox.frag" }
{
    initReverseFBO(screenWidth, screenHeight);
    initCubeVertices();
    initScreenQuad();
    mRearViewMatrix = glm::mat4(1);
    mRearViewMatrix = glm::translate(mRearViewMatrix, { 0, 0.8, 0 });
    mRearViewMatrix = glm::scale(mRearViewMatrix, { 0.2, 0.2, 1 });
}

void Renderer::render(App& app, Option option) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    switch (option) {
    case Option::complete:
        renderLighting(app);
        renderLightSources(app);
        renderSkyBox(app);

        glBindFramebuffer(GL_FRAMEBUFFER, mReverseFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        app.mCamera.setForward(app.mCamera.getForward() * -1.0f);

        renderLighting(app);
        renderLightSources(app);
        renderSkyBox(app);

        app.mCamera.setForward(app.mCamera.getForward() * -1.0f);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        renderScreenQuad(app, mReverseColorTex, true);
    }
}