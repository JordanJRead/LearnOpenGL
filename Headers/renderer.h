#pragma once
#include "OpenGL Wrappers/FBO.h"
#include "OpenGL Wrappers/TEX.h"
#include "OpenGL Wrappers/RBO.h"
#include "OpenGL Wrappers/VAO.h"
#include "OpenGL Wrappers/BUF.h"
#include "textureutils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader Classes/bordershader.h"
#include "Shader Classes/lightingshader.h"
#include "Shader Classes/lightsourceshader.h"
#include "Shader Classes/screenquadshader.h"
#include "Shader Classes/skyboxshader.h"
#include "dynamiccubemap.h"
#include <array>
#include "cubemap.h"

class App;
class Model;

class Renderer {
public:
    enum Option {
        complete,
        testTexture,
        border
    };

private:
    BUF mMatricesBuffer;

    FBO mDynamicCubeMapFBO;
    TEX mDynamicCubeMapColorTex;
    RBO mDynamicCubeMapDepthStencilRBO;

    CubeMap mBlackCubeMap;

    std::vector<DynamicCubeMap> mDynamicCubeMaps;

    VAO mCubeVAO;
    BUF mCubeVBO;

    VAO mScreenQuadVAO;
    BUF mScreenQuadVBO;

    glm::mat4 mRearViewMatrix;

    LightingShader mLightingShader;
    LightSourceShader mLightSourceShader;
    BorderShader mBorderShader;
    ScreenQuadShader mScreenQuadShader;
    SkyBoxShader mSkyBoxShader;
    FBO mReverseFBO;
    TEX mReverseColorTex;
    RBO mReverseDepthStencilRBO;
    TextureUtils::DefaultTextures mDefaultTextures;

    DynamicCubeMap createDynamicCubeMap(const glm::vec3& pos, const Scene& scene, int modelIndex);

    void renderLightingModel(const Model& model, const TEX& environmentCubeMapTex);
    void renderLighting(const Camera& camera, const Scene& scene, Option option = Option::complete, int ignoreModelIndex = -1);

    void renderLightSources(App& app);

    void renderSkyBox(App& app, unsigned int skyBoxTexID);

    void renderScreenQuad(unsigned int texID, bool rearView);

    void renderBorders(const Camera& camera, const Scene& scene);

    void initReverseFBO(int screenWidth, int screenHeight);
    void initCubeVertices();
    void initScreenQuad();
    void initDynamicEnvironment();
    void initMatricesBuffer();

public:
    void startBlurEffect();
    void createDynamicEnvironments(App& app);
    Renderer(int screenWidth, int screenHeight, App& app);
	void render(App&, Option renderOption);
};