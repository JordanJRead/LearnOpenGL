#include "renderer.h"
#include "app.h"
#include "texturetype.h"

void Renderer::startBlurEffect() {
    mScreenQuadShader.startEffect();
}

/// <summary>
/// Sets up dynamic cube maps for each object in the scene which requires it
/// </summary>
void Renderer::createDynamicCubeMaps(Scene& scene) {
    for (size_t i{ 0 }; i < scene.getModels().size(); ++i) {
        Model& model = scene.getModel(i);
        if (model.mUsesDynamicEnvironment) {
            DynamicCubeMap dynamicCubeMap = createDynamicCubeMap(model.mTransform.pos, scene, i);
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

/// <summary>
/// Creates a dynamic cube map
/// </summary>
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
        renderEntireSceneLighting(mockCamera, scene, true, modelIndex);
        cubeMap.setFace(mDynamicCubeMapColorTex, i);
    }
    glViewport(0, 0, 800, 600); // todo
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return cubeMap;
}

/// <summary>
/// Creates borders around objects that use them and have already drew in the stencil buffer
/// </summary>
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

void Renderer::renderEntireSceneLighting(const Camera& camera, const Scene& scene, bool drawBorders, int ignoreModelIndex = -1) {
    mLightingShader.use();
    mLightingShader.setPerFrameUniforms(camera, scene);

    const std::vector<Model>& models{ scene.getModels() };
    for (int i{ 0 }; i < models.size(); ++i) {
        if (i == ignoreModelIndex) continue;

        const Model& model{ models[i] };
        if (drawBorders && model.mHasBorder) {
            glStencilMask(0xFF);
        }
        else {
            glStencilMask(0x00);
        }
        
        // We don't want to render objects that should have a dynamic environment map, but don't.
        // This is so that when creating the dynamic environment map for an object, that object won't see itself
        if (model.mUsesDynamicEnvironment && model.mDynamicEnvironmentIndex >= 0) {
            mLightingShader.renderModel(model, mDynamicCubeMaps[model.mDynamicEnvironmentIndex].mTEX, mDefaultTextures);
        }
        else{// if (!model.mUsesDynamicEnvironment){
            mLightingShader.renderModel(model, scene.getCubeMap().mTEX, mDefaultTextures);
        }
    }

    // Transparent objects
    const std::vector<Model>& transparentModels{ scene.getTransparentModels() };
    for (int i{ 0 }; i < transparentModels.size(); ++i) {
        const Model& model{ transparentModels[i] };
        if (drawBorders && model.mHasBorder) {
            glStencilMask(0xFF);
        }
        else {
            glStencilMask(0x00);
        }
        if (model.mUsesDynamicEnvironment && model.mDynamicEnvironmentIndex >= 0) {
            mLightingShader.renderModel(model, mDynamicCubeMaps[model.mDynamicEnvironmentIndex].mTEX, mDefaultTextures);
        }
        else if (!model.mUsesDynamicEnvironment) {
            mLightingShader.renderModel(model, scene.getCubeMap().mTEX, mDefaultTextures);
        }
    }
    glBindVertexArray(0);
}

/// <summary>
/// Renders every light source as a cube
/// </summary>
void Renderer::renderLightSources(const Camera& camera, const Scene& scene) {
    mLightSourceShader.use();
    mLightSourceShader.setUniformView(camera.getView());
    mLightSourceShader.setUniformProjection(camera.getProjection());

    for (const PointLight& pointLight : scene.getPointLights()) {
        glBindVertexArray(mCubeVAO);
        mLightSourceShader.setUniformModel(pointLight.modelInfo.model);
        mLightSourceShader.setUniformLightColor(pointLight.colors.diffuse);
        glDrawArrays(GL_TRIANGLES, 0, pointLight.modelInfo.vertexCount);
    }
    for (const SpotLight& spotLight : scene.getSpotLights()) {
        glBindVertexArray(mCubeVAO);
        mLightSourceShader.setUniformModel(spotLight.modelInfo.model);
        mLightSourceShader.setUniformLightColor(spotLight.colors.diffuse);
        glDrawArrays(GL_TRIANGLES, 0, spotLight.modelInfo.vertexCount);
    }
}

void Renderer::renderEntireSceneGouraud(const Camera& camera, const Scene& scene) {
    // Per-frame uniforms
    mGouraudShader.use();
    mGouraudShader.setPerFrameUniforms(camera, scene);

    const std::vector<Model>& models{ scene.getModels() };
    for (int i{ 0 }; i < models.size(); ++i) {

        const Model& model{ models[i] };

        // We don't want to render objects that should have a dynamic environment map, but don't.
        // This is so that when creating the dynamic environment map for an object, that object won't see itself
        if (model.mUsesDynamicEnvironment && model.mDynamicEnvironmentIndex >= 0) {
            mLightingShader.renderModel(model, mDynamicCubeMaps[model.mDynamicEnvironmentIndex].mTEX, mDefaultTextures);
        }
        else {// if (!model.mUsesDynamicEnvironment){
            mLightingShader.renderModel(model, scene.getCubeMap().mTEX, mDefaultTextures);
        }
    }

    // Transparent objects
    const std::vector<Model>& transparentModels{ scene.getTransparentModels() };
    for (int i{ 0 }; i < transparentModels.size(); ++i) {
        const Model& model{ transparentModels[i] };

        if (model.mUsesDynamicEnvironment && model.mDynamicEnvironmentIndex >= 0) {
            mLightingShader.renderModel(model, mDynamicCubeMaps[model.mDynamicEnvironmentIndex].mTEX, mDefaultTextures);
        }
        else if (!model.mUsesDynamicEnvironment) {
            mLightingShader.renderModel(model, scene.getCubeMap().mTEX, mDefaultTextures);
        }
    }
    glBindVertexArray(0);
}

void Renderer::renderSkyBox(const Camera& camera, unsigned int skyBoxTexID) {
    mSkyBoxShader.use();
    //glDisable(GL_CULL_FACE); // todo
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(mCubeVAO);    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTexID);
    mSkyBoxShader.setUniformView(glm::mat4(glm::mat3(camera.getView())));
    mSkyBoxShader.setUniformProjection(camera.getProjection());
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

/// <summary>
/// Renders a texture onto the currently bound framebuffer
/// </summary>
void Renderer::renderScreenQuad(unsigned int texID, bool quadAtTopOfScreen = false) {
    mScreenQuadShader.use();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE); // todo

    mScreenQuadShader.setUniformOffset(mScreenQuadShader.getOffset(mScreenQuadShader.mEffectStartTime));
    if (quadAtTopOfScreen) {
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
    , mGouraudShader{ "shaders/gouraud.vert", "shaders/gouraud.frag" }
    , mInstancedShader{ "shaders/instanced.vert", "shaders/instanced.frag "}
{
    initCubeVertices();
    initScreenQuad();
    initDynamicEnvironment();
    initMatricesBuffer();
    mRearViewMatrix = glm::mat4(1);
    mRearViewMatrix = glm::translate(mRearViewMatrix, { 0, 0.8, 0 });
    mRearViewMatrix = glm::scale(mRearViewMatrix, { 0.2, 0.2, 1 });
}

void Renderer::renderScene(const Camera& camera, const Scene& scene, bool drawBorders) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glStencilMask(0xFF);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);

    renderEntireSceneLighting(camera, scene, true);
    //renderEntireSceneGouraud(camera, scene);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    //glDisable(GL_DEPTH_TEST);
    renderBorders(camera, scene);
    //glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);

    renderLightSources(camera, scene);
    renderSkyBox(camera, scene.getCubeMap().mTEX);
}

void Renderer::renderInstanced(const Camera& camera, const Scene& scene) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    mInstancedShader.use();
    mInstancedShader.setPerFrameUniforms(camera);
    mInstancedShader.renderModel(300, scene.getInstancedModel(), mDefaultTextures);
}