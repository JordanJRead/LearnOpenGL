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
#include "Shader Classes/gouraudshader.h"
#include "Shader Classes/instancedshader.h"
#include "dynamiccubemap.h"
#include <array>
#include "cubemap.h"

class App;
class Model;

class Renderer {
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
    GouraudShader mGouraudShader;
    InstancedShader mInstancedShader;

    TextureUtils::DefaultTextures mDefaultTextures;

    DynamicCubeMap createDynamicCubeMap(const glm::vec3& pos, const Scene& scene, int modelIndex);

    void renderEntireSceneLighting(const Camera& camera, const Scene& scene, bool drawBorders, int ignoreModelIndex);

    void renderEntireSceneGouraud(const Camera& camera, const Scene& scene);

    void renderLightSources(const Camera& camera, const Scene& scene);

    void renderSkyBox(const Camera& camera, unsigned int skyBoxTexID);

    void renderScreenQuad(unsigned int texID, bool quadAtTopOfScreen);

    void renderBorders(const Camera& camera, const Scene& scene);

    void initCubeVertices();
    void initScreenQuad();
    void initDynamicEnvironment();
    void initMatricesBuffer();

public:
    void startBlurEffect();
    void createDynamicCubeMaps(Scene& scene);
    Renderer(int screenWidth, int screenHeight, App& app);
	void renderScene(const Camera& camera, const Scene& scene, bool drawBorders);
    void renderInstanced(const Camera& camera, const Scene& scene);
};