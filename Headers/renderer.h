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
#include "Shader Classes/shownormalsshader.h"
#include "dynamiccubemap.h"
#include <array>
#include "cubemap.h"
#include "matrixuniformbuffer.h"

class App;
class Model;

class Renderer {
public:
    Renderer(int screenWidth, int screenHeight, App& app);
    void startBlurEffect();
    void createDynamicCubeMaps(Scene& scene, const Camera& mainCamera);
    void renderScene(const Camera& camera, const Scene& scene, bool drawBorders);
    void renderInstanced(const Camera& camera, const Scene& scene);
    void renderGeometry();
    void renderNormals(const Scene& scene);

private:
    MatrixUniformBuffer mMatrixUniformBuffer;

    StandardFramebuffer mDynamicCubeMapTemporaryFramebuffer;

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
    ShowNormalsShader mShowNormalsShader{ "shaders/shownormals.vert", "shaders/shownormals.geom", "shaders/shownormals.frag" };

    DynamicCubeMap createDynamicCubeMap(const glm::vec3& pos, const Scene& scene, int modelIndex, const Camera& mainCamera);

    void renderEntireSceneLighting(const Camera& camera, const Scene& scene, bool drawBorders, int ignoreModelIndex);

    void renderEntireSceneGouraud(const Camera& camera, const Scene& scene);

    void renderLightSources(const Scene& scene);

    void renderSkyBox(unsigned int skyBoxTexID);

    void renderScreenQuad(unsigned int texID, bool quadAtTopOfScreen);

    void renderBorders(const Scene& scene);

    void initCubeVertices();
    void initScreenQuad();
};