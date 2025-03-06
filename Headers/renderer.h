#pragma once
#include "OpenGL Wrappers/FBO.h"
#include "OpenGL Wrappers/TEX.h"
#include "OpenGL Wrappers/RBO.h"
#include "OpenGL Wrappers/VAO.h"
#include "OpenGL Wrappers/VBO.h"
#include "textureutils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader Classes/bordershader.h"
#include "Shader Classes/lightingshader.h"
#include "Shader Classes/lightsourceshader.h"
#include "Shader Classes/screenquadshader.h"
#include "Shader Classes/skyboxshader.h"

class App;
class Model;

class Renderer {
    friend class App;
private:
    VAO mCubeVAO;
    VBO mCubeVBO;

    VAO mScreenQuadVAO;
    VBO mScreenQuadVBO;

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

    void renderLightingModel(const Model& model);
    void renderLighting(App& app);

    void renderLightSources(App& app);

    void renderSkyBox(App& app);

    void renderScreenQuad(App& app, unsigned int texID, bool rearView);

public:
    enum Option {
        complete
    };
    Renderer(int screenWidth, int screenHeight);
	void render(App&, Option renderOption);
    void initReverseFBO(int screenWidth, int screenHeight);
    void initCubeVertices();
    void initScreenQuad();
};