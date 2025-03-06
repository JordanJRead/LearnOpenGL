#include "skyboxshader.h"
#include "../OpenGL Wrappers/VAO.h"
#include "../OpenGL Wrappers/VBO.h"
#include <string_view>
#include <vector>
#include "../camera.h"
#include "../scene.h"

void SkyBoxShader::setUniformView(const glm::mat4& view) const {
	setMatrix4("view", view);
}
void SkyBoxShader::setUniformProjection(const glm::mat4& projection) const {
	setMatrix4("projection", projection);
}
void SkyBoxShader::setUniformSkyBoxTexture(int unit) const {
	setInt("cubeMap", unit);
}
SkyBoxShader::SkyBoxShader(std::string_view vertPath, std::string_view fragPath) : Shader{ vertPath, fragPath } {
    use();
    setUniformSkyBoxTexture(0);
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

void SkyBoxShader::render(const Scene& scene, const Camera& camera) {
    use();
    glDisable(GL_CULL_FACE);
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(mCubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, scene.getCubeMap().mTEX);
    setUniformView(glm::mat4(glm::mat3(camera.getView())));
    setUniformProjection(camera.getProjection());
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}