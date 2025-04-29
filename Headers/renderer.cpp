#include "renderer.h"
#include "app.h"
#include "OpenGL Wrappers/VAO.h";
#include "OpenGL Wrappers/BUF.h";
#include "uc_apng_loader.h"
#include "texture2dmanager.h"
#include "textureutils.h"
#include "cstdint"
extern Texture2DManager* gTexture2DManager;

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
            DynamicCubeMap dynamicCubeMap = createDynamicCubeMap(model.mTransform.pos, scene, i, mainCamera);
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
    glBindFramebuffer(GL_FRAMEBUFFER, mDynamicCubeMapFBO); // also render the skybox in this function too
    Camera mockCamera{ 1024, 1024, pos, 90, 0, 0 }; //fixme todo set in the matrix buffer
    glViewport(0, 0, 1024, 1024);
    mockCamera.setUp({ 0, -1, 0 });
    for (int i{ 0 }; i < 6; ++i) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        if (i == 1) {
            mockCamera.setYaw(180);
        }
        else if (i == 2) {
            mockCamera.setYaw(90);
            mockCamera.setPitch(90);
        }
        else if (i == 3) {
            mockCamera.setPitch(-90);
        }
        else if (i == 4) {
            mockCamera.setPitch(0);
            mockCamera.setYaw(-90);
        }
        else if (i == 5) {
            mockCamera.setYaw(90);
        }
        mMatrixUniformBuffer.setAllMatrices(mockCamera);
        renderEntireSceneLighting(mockCamera, scene, true, modelIndex);
        renderLightSources(scene);
        renderSkyBox(scene.getCubeMap().mTEX);
        cubeMap.setFace(mDynamicCubeMapColorTex, i);
    }
    mMatrixUniformBuffer.setAllMatrices(mainCamera);
    glViewport(0, 0, mainCamera.mScreenWidth, mainCamera.mScreenHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return cubeMap;
}

/// <summary>
/// Creates borders around objects that use them and have already drew in the stencil buffer
/// </summary>
void Renderer::renderBorders(const Scene& scene) {
    mBorderShader.use();
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
void Renderer::renderLightSources(const Scene& scene) {
    mLightSourceShader.use();

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
    , mBlackCubeMap{ {"images/black.png", "images/black.png", "images/black.png", "images/black.png", "images/black.png", "images/black.png"}}
    , mGouraudShader{ "shaders/gouraud.vert", "shaders/gouraud.frag" }
    , mInstancedShader{ "shaders/instanced.vert", "shaders/instanced.frag "}
    , mMatrixUniformBuffer{ 0 }
{
    mLightingShader.use();
    //mLightingShader.setUniformDoExploding(true);
    initCubeVertices();
    initScreenQuad();
    initDynamicEnvironment();
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

void Renderer::renderScene(const Camera& camera, const Scene& scene, bool drawBorders) {
    mMatrixUniformBuffer.setViewMatrix(camera.getView());
    mMatrixUniformBuffer.setProjectionMatrix(camera.getProjection());

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
    renderBorders(scene);
    glDisable(GL_STENCIL_TEST);

    renderLightSources(scene);
    renderSkyBox(scene.getCubeMap().mTEX);
    //renderSkyBox(mDynamicCubeMaps[0].mTEX);

    //renderNormals(scene);
}

void Renderer::renderInstanced(const Camera& camera, const Scene& scene) {
    mMatrixUniformBuffer.setViewMatrix(camera.getView());
    mMatrixUniformBuffer.setProjectionMatrix(camera.getProjection());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    mInstancedShader.use();
    mInstancedShader.renderModel(250, scene.getInstancedModel(), mDefaultTextures);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
}

void Renderer::renderGeometry() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

void Renderer::renderGif() {
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //auto loader = uc::apng::create_file_loader("C:/Users/jorda/source/repos/JordanJRead/LearnOpenGL/images/gifs/earth2.png");
    //std::vector<uc::apng::frame> frames;
    //while (loader.has_frame()) {
    //    frames.push_back(loader.next_frame());
    //}
    //TEX frameOne;
    //glBindTexture(GL_TEXTURE_2D, frameOne);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frames[0].image.width(), frames[0].image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, frames[0].image.data());

    //unsigned char* testData = new uint8_t[3 * 1 * 4];

    //for (int i = 0; i < 12; ++i) {
    //    testData[i] = 255;
    //    std::cerr << static_cast<int>(testData[i]) << "\n";
    //}



    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 3, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, testData);
    //glGenerateMipmap(GL_TEXTURE_2D);
    //delete[] testData;

    //renderScreenQuad(frameOne);
}