#include "renderer.h"
#include "app.h"
#include "OpenGL Wrappers/VAO.h";
#include "OpenGL Wrappers/BUF.h";
#include "Shader Classes/gammashader.h"
#include "texture2dmanager.h"

void Renderer::startBlurEffect() {
    mScreenQuadShader.startEffect();
}

/// <summary>
/// Sets up dynamic cube maps for each object in the scene which requires it
/// </summary>
void Renderer::createDynamicCubeMaps(Scene& scene, const Camera& mainCamera) {
    for (size_t i{ 0 }; i < scene.getModels().size(); ++i) {
        Model& model = scene.getModel(i);
        if (model.mUsesDynamicEnvironment) {
            DynamicCubeMap dynamicCubeMap = createDynamicCubeMap(model.mTransform.getPosition(), scene, i, mainCamera);
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
DynamicCubeMap Renderer::createDynamicCubeMap(const glm::vec3& pos, const Scene& scene, int modelIndex, const Camera& mainCamera) {
    DynamicCubeMap cubeMap;
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap.mTEX);
    mDynamicCubeMapTemporaryFramebuffer.use();
    Camera mockCamera{ mDynamicCubeMapTemporaryFramebuffer.getImageWidth(), mDynamicCubeMapTemporaryFramebuffer.getImageHeight(), pos, 90, 0, 0}; //fixme todo set in the matrix buffer
    glViewport(0, 0, mDynamicCubeMapTemporaryFramebuffer.getImageWidth(), mDynamicCubeMapTemporaryFramebuffer.getImageHeight());
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
        mMatrixUniformBuffer.setAllMatrices(mockCamera);

        renderEntireSceneLighting(mockCamera, scene, true, modelIndex);
        renderSkyBox(scene.getSkyBoxCubeMap().mTEX);
        cubeMap.setFace(mDynamicCubeMapTemporaryFramebuffer, i);
    }
    mMatrixUniformBuffer.setAllMatrices(mainCamera);
    glViewport(0, 0, 800, 600); // todo
    glBindFramebuffer(GL_FRAMEBUFFER, mMainFramebuffer);
    return cubeMap;
}

/// <summary>
/// Creates borders around objects that use them and have already drew in the stencil buffer
/// </summary>
void Renderer::renderBorders(const Scene& scene) {
    mBorderShader.use();
    for (const Model& model : scene.getModels()) {
        if (model.mHasBorder) {
            mBorderShader.setUniformModel(model.mTransform.getModelMatrix());
            for (const Mesh& mesh : model.getMeshes()) {
                glBindVertexArray(mesh.mVAO);
                glDrawElements(GL_TRIANGLES, mesh.mVertexCount, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
        }
    }

    for (const Model& model : scene.getTransparentModels()) {
        if (model.mHasBorder) {
            mBorderShader.setUniformModel(model.mTransform.getModelMatrix());
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
            mLightingShader.renderModel(model, mDynamicCubeMaps[model.mDynamicEnvironmentIndex].mTEX);
        }
        else{// if (!model.mUsesDynamicEnvironment){
            mLightingShader.renderModel(model, scene.getSkyBoxCubeMap().mTEX);
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
            mLightingShader.renderModel(model, mDynamicCubeMaps[model.mDynamicEnvironmentIndex].mTEX);
        }
        else if (!model.mUsesDynamicEnvironment) {
            mLightingShader.renderModel(model, scene.getSkyBoxCubeMap().mTEX);
        }
    }
    glBindVertexArray(0);
}

/// <summary>
/// Renders every light source as a cube
/// </summary>
void Renderer::renderLightSources(const Scene& scene) {
    mLightSourceShader.use();

    for (const PointLight& pointLight : scene.getPointLights()) {
        glBindVertexArray(mCubeVAO);
        mLightSourceShader.setUniformPosition(pointLight.mPosition);
        mLightSourceShader.setUniformLightColor(pointLight.mColor);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    for (const SpotLight& spotLight : scene.getSpotLights()) {
        glBindVertexArray(mCubeVAO);
        mLightSourceShader.setUniformPosition(spotLight.mPosition);
        mLightSourceShader.setUniformLightColor(spotLight.mColor);
        glDrawArrays(GL_TRIANGLES, 0, 36);
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
            mLightingShader.renderModel(model, mDynamicCubeMaps[model.mDynamicEnvironmentIndex].mTEX);
        }
        else {// if (!model.mUsesDynamicEnvironment){
            mLightingShader.renderModel(model, scene.getSkyBoxCubeMap().mTEX);
        }
    }

    // Transparent objects
    const std::vector<Model>& transparentModels{ scene.getTransparentModels() };
    for (int i{ 0 }; i < transparentModels.size(); ++i) {
        const Model& model{ transparentModels[i] };

        if (model.mUsesDynamicEnvironment && model.mDynamicEnvironmentIndex >= 0) {
            mLightingShader.renderModel(model, mDynamicCubeMaps[model.mDynamicEnvironmentIndex].mTEX);
        }
        else if (!model.mUsesDynamicEnvironment) {
            mLightingShader.renderModel(model, scene.getSkyBoxCubeMap().mTEX);
        }
    }
    glBindVertexArray(0);
}

void Renderer::renderSkyBox(unsigned int skyBoxTexID) {
    mSkyBoxShader.use();
    //glDisable(GL_CULL_FACE); // todo
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(mCubeVAO);    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTexID);
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
    : mLightingShader{ "shaders/lighting.vert", "shaders/lighting.geom", "shaders/lighting.frag"}
    , mLightSourceShader{ "shaders/lightSource.vert", "shaders/lightsource.frag" }
    , mBorderShader{ "shaders/border.vert", "shaders/border.frag" }
    , mScreenQuadShader{ "shaders/screenquad.vert", "shaders/screenquad.frag" }
    , mSkyBoxShader{ "shaders/skybox.vert", "shaders/skybox.frag" }
    , mGouraudShader{ "shaders/gouraud.vert", "shaders/gouraud.frag" }
    , mInstancedShader{ "shaders/instanced.vert", "shaders/instanced.frag "}
    , mMatrixUniformBuffer{ 0 }
    , mDynamicCubeMapTemporaryFramebuffer{ 128, 128 }
    , mMainFramebuffer{ screenWidth, screenHeight }
    , mGammaCorrectionShader{ "shaders/gamma.vert", "shaders/gamma.frag", mScreenQuadVAO }
{
    mLightingShader.use();
    //mLightingShader.setUniformDoExploding(true);
    initCubeVertices();
    initScreenQuad();
    mRearViewMatrix = glm::mat4(1);
    mRearViewMatrix = glm::translate(mRearViewMatrix, { 0, 0.8, 0 });
    mRearViewMatrix = glm::scale(mRearViewMatrix, { 0.2, 0.2, 1 });
}

void Renderer::renderNormals(const Scene& scene) {
    mShowNormalsShader.use();
    for (const Model& model : scene.getModels()) {
        mShowNormalsShader.renderModel(model);
    }
}

extern Texture2DManager* gTexture2DManager;
void Renderer::renderScene(const Camera& camera, const Scene& scene, bool drawBorders) {

    //glClear(GL_DEPTH_BUFFER_BIT);
    //glEnable(GL_DEPTH);
    //glViewport(0, 0, 1024, 1024);
    //const ShadowCaster& shadowCaster{ scene.getShadowCaster() };
    //mMatrixUniformBuffer.setViewMatrix(shadowCaster.getViewMatrix());
    //mMatrixUniformBuffer.setProjectionMatrix(shadowCaster.getProjectionMatrix());
    //mDepthShader.use();
    //shadowCaster.useFramebuffer();
    //for (const Model& model : scene.getModels()) {
    //    mDepthShader.RenderModel(model);
    //}
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glViewport(0, 0, 800, 600);
    //renderScreenQuad(shadowCaster.getDepthTexture());
    //return;


    mMatrixUniformBuffer.setViewMatrix(camera.getView());
    mMatrixUniformBuffer.setProjectionMatrix(camera.getProjection());

    glBindFramebuffer(GL_FRAMEBUFFER, mMainFramebuffer);
    glStencilMask(0xFF);
    glClea  r(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);

    renderEntireSceneLighting(camera, scene, true);
    //renderEntireSceneGouraud(camera, scene);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    renderBorders(scene);
    glDisable(GL_STENCIL_TEST);

    renderLightSources(scene);
    renderSkyBox(scene.getSkyBoxCubeMap().mTEX);

    // Works
    std::vector<GLubyte> data{ mMainFramebuffer.getImageData() };


    const TEX& mainRenderTex{ mMainFramebuffer.getColorTex() };
    glBindTexture(GL_TEXTURE_2D, mainRenderTex);
    unsigned int byteCount{ static_cast<unsigned int>(800) * static_cast<unsigned int>(600) * 4 * sizeof(GLubyte) };
    std::vector<GLubyte> imageData(byteCount);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(imageData[0])); // Doesn't work

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    mGammaCorrectionShader.Correct(mainRenderTex);
    //renderScreenQuad(gTexture2DManager->getTexture(-1, TextureUtils::Type::diffuse));
    //renderScreenQuad(mainRenderTex);
}

void Renderer::renderInstanced(const Camera& camera, const Scene& scene) {
    mMatrixUniformBuffer.setViewMatrix(camera.getView());
    mMatrixUniformBuffer.setProjectionMatrix(camera.getProjection());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    mInstancedShader.use();
    mInstancedShader.renderModel(300, scene.getInstancedModel());
}

void Renderer::renderGeometry() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    static float points[] = {
        -0.5f, 0.5, 1, 0, 0,
        0.5, 0.5, 0, 1, 0,
        0.5f, -0.5f, 0, 0, 1,
        -0.5f, -0.5f, 1, 1, 1
    };

    static Shader shader{ "shaders/geometry.vert", "shaders/geometry.geom", "shaders/geometry.frag" };

    VAO vao;
    glBindVertexArray(vao);

    BUF vbo;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, std::size(points) * sizeof(float), &(points[0]), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(float) * 2));

    shader.use();
    glDrawArrays(GL_POINTS, 0, 4);
}