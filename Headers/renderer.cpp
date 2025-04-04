#include "renderer.h"
#include "app.h"

void Renderer::startBlurEffect() {
    mScreenQuadShader.startEffect();
}

void Renderer::createDynamicEnvironments(App& app) {
    for (size_t i{ 0 }; i < app.mScene.getModels().size(); ++i) {
        Model& model = app.mScene.getModel(i);
        if (model.mUsesDynamicEnvironment) {
            DynamicCubeMap dynamicCubeMap = createDynamicCubeMap(model.mTransform.pos, app.mScene, i);
            if (model.mDynamicEnvironmentIndex == -1) {
                mDynamicCubeMaps.push_back(std::move(dynamicCubeMap));
                model.mDynamicEnvironmentIndex = mDynamicCubeMaps.size() - 1;
            }
            else {
                mDynamicCubeMaps[model.mDynamicEnvironmentIndex] = std::move(dynamicCubeMap);
            }
        }
    }
}

DynamicCubeMap Renderer::createDynamicCubeMap(const glm::vec3& pos, const Scene& scene, int modelIndex) {
    DynamicCubeMap cubeMap;
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap.mTEX);
    glBindFramebuffer(GL_FRAMEBUFFER, mDynamicCubeMapFBO);
    Camera mockCamera{ 1024, 1024, pos, 90, 0, 0 };
    glViewport(0, 0, 1024, 1024);
    mockCamera.setUp({ 0, -1, 0 }); // not sure why
    for (int i{ 0 }; i < 6; ++i) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        if (i == 1) {
            mockCamera.setYaw(180);
        }
        else if (i == 2) {
            mockCamera.setUp({ 0, 0, 1 });
            mockCamera.setYaw(270);
            mockCamera.setPitch(90);
        }
        else if (i == 3) {
            mockCamera.setUp({ 0, 0, -1 });
            mockCamera.setPitch(-90);
        }
        else if (i == 4) {
            mockCamera.setUp({ 0, -1, 0 }); // not sure why
            mockCamera.setPitch(0);
            mockCamera.setYaw(90);
        }
        else if (i == 5) {
            mockCamera.setYaw(-90);
        }
        renderLighting(mockCamera, scene, Option::complete, modelIndex);
        cubeMap.setFace(mDynamicCubeMapColorTex, i);
    }
    glViewport(0, 0, 800, 600); // todo
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return cubeMap;
}

void Renderer::renderBorders(const Camera& camera, const Scene& scene) {
    mBorderShader.use();
    mBorderShader.setUniformView(camera.getView());
    mBorderShader.setUniformProjection(camera.getProjection());
    for (const Model& model : scene.getModels()) {
        if (model.mHasBorder) {
            mBorderShader.setUniformModel(model.mModel);
            for (const Mesh& mesh : model.getMeshes()) {
                glBindVertexArray(mesh.mVAO);
                glDrawElements(GL_TRIANGLES, mesh.mVertexCount, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
        }
    }

    for (const Model& model : scene.getTransparentModels()) {
        if (model.mHasBorder) {
            mBorderShader.setUniformModel(model.mModel);
            for (const Mesh& mesh : model.getMeshes()) {
                glBindVertexArray(mesh.mVAO);
                glDrawElements(GL_TRIANGLES, mesh.mVertexCount, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
        }
    }
}

void Renderer::renderLightingModel(const Model& model, const TEX& environmentCubeMapTex) {
    mLightingShader.setUniformModel(model.mModel);
    glBindTexture(GL_TEXTURE_CUBE_MAP, environmentCubeMapTex);
    for (const Mesh& mesh : model.getMeshes()) {
        mLightingShader.setUniformMaterialShininess(mesh.mShininess == 0 ? 1 : mesh.mShininess);
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

void Renderer::renderLighting(const Camera& camera, const Scene& scene, Option option, int ignoreModelIndex) {
    // Per-frame uniforms
    mLightingShader.use();
    mLightingShader.setUniformView(camera.getView());
    mLightingShader.setUniformProjection(camera.getProjection());
    mLightingShader.setUniformViewPos(camera.getPos());
    mLightingShader.setUniformViewDir(camera.getForward());
    mLightingShader.setUniformPointLights(scene.getPointLights());
    mLightingShader.setUniformMaxPointLights(scene.getPointLights().size());
    mLightingShader.setUniformDirLight(scene.getDirLight());
    mLightingShader.setUniformSpotLights(scene.getSpotLights());
    mLightingShader.setUniformMaxSpotLights(scene.getSpotLights().size());

    const std::vector<Model>& models{ scene.getModels() };
    for (int i{ 0 }; i < models.size(); ++i) {
        if (i == ignoreModelIndex) continue;

        const Model& model{ models[i] };
        if (option == Option::border && model.mHasBorder) {
            glStencilMask(0xFF);
        }
        else {
            glStencilMask(0x00);
        }
        glActiveTexture(GL_TEXTURE0 + mLightingShader.mSkyboxTextureIndex);
        
        // We don't want to render objects that should have a dynamic environment map, but don't.
        // This is so that when creating the dynamic environment map for an object, that object won't see itself
        if (model.mUsesDynamicEnvironment && model.mDynamicEnvironmentIndex >= 0) {
            renderLightingModel(model, mDynamicCubeMaps[model.mDynamicEnvironmentIndex].mTEX);
        }
        else{// if (!model.mUsesDynamicEnvironment){
            renderLightingModel(model, scene.getCubeMap().mTEX);
        }
    }

    const std::vector<Model>& transparentModels{ scene.getModels() };
    for (int i{ 0 }; i < transparentModels.size(); ++i) {
        const Model& model{ transparentModels[i] };
        if (option == Option::border && model.mHasBorder) {
            glStencilMask(0xFF);
        }
        else {
            glStencilMask(0x00);
        }
        glActiveTexture(GL_TEXTURE0 + mLightingShader.mSkyboxTextureIndex);
        if (model.mUsesDynamicEnvironment && model.mDynamicEnvironmentIndex >= 0) {
            renderLightingModel(model, mDynamicCubeMaps[model.mDynamicEnvironmentIndex].mTEX);
        }
        else if (!model.mUsesDynamicEnvironment) {
            renderLightingModel(model, scene.getCubeMap().mTEX);
        }
    }
    glBindVertexArray(0);
}

void Renderer::renderLightSources(App& app) {
    mLightSourceShader.use();
    mLightSourceShader.setUniformView(app.mCamera.getView());
    mLightSourceShader.setUniformProjection(app.mCamera.getProjection());

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

void Renderer::renderSkyBox(App& app, unsigned int skyBoxTexID) {
    mSkyBoxShader.use();
    //glDisable(GL_CULL_FACE); // todo
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(mCubeVAO);    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTexID);
    mSkyBoxShader.setUniformView(glm::mat4(glm::mat3(app.mCamera.getView())));
    mSkyBoxShader.setUniformProjection(app.mCamera.getProjection());
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Renderer::renderScreenQuad(unsigned int texID, bool rearView = false) {
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

Renderer::Renderer(int screenWidth, int screenHeight, App& app)
    : mLightingShader{ "shaders/lighting.vert", "shaders/lighting.frag" }
    , mLightSourceShader{ "shaders/lightSource.vert", "shaders/lightsource.frag" }
    , mBorderShader{ "shaders/border.vert", "shaders/border.frag" }
    , mScreenQuadShader{ "shaders/screenquad.vert", "shaders/screenquad.frag" }
    , mSkyBoxShader{ "shaders/skybox.vert", "shaders/skybox.frag" }
    , mBlackCubeMap{ {"images/black.png", "images/black.png", "images/black.png", "images/black.png", "images/black.png", "images/black.png"}}
{
    initReverseFBO(screenWidth, screenHeight);
    initCubeVertices();
    initScreenQuad();
    initDynamicEnvironment();
    initMatricesBuffer();
    mRearViewMatrix = glm::mat4(1);
    mRearViewMatrix = glm::translate(mRearViewMatrix, { 0, 0.8, 0 });
    mRearViewMatrix = glm::scale(mRearViewMatrix, { 0.2, 0.2, 1 });
}

void Renderer::render(App& app, Option option) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glStencilMask(0xFF);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    switch (option) {
    case Option::complete:
        renderLighting(app.mCamera, app.mScene);
        renderLightSources(app);
        renderSkyBox(app, app.mScene.getCubeMap().mTEX);

        glBindFramebuffer(GL_FRAMEBUFFER, mReverseFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        app.mCamera.setForward(app.mCamera.getForward() * -1.0f);

        renderLighting(app.mCamera, app.mScene);
        renderLightSources(app);
        renderSkyBox(app, app.mScene.getCubeMap().mTEX);

        app.mCamera.setForward(app.mCamera.getForward() * -1.0f);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        renderScreenQuad(mReverseColorTex, true);
        break;

    case Option::testTexture:
        renderScreenQuad(mDefaultTextures.diffuse.mTex.mID, false);
        break;

    case Option::border:
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        renderLighting(app.mCamera, app.mScene, Option::border);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        renderBorders(app.mCamera, app.mScene);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
    }
}