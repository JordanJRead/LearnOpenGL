#pragma once
#include "shader.h"
#include "scene.h"
#include "camera.h"
#include "stb_image.h"
//unsigned int textureFromFile(std::string_view imagePath);
class TextureShader : public Shader {
public:
	TextureShader(const std::string& vertexPath, const std::string& fragPath): Shader{vertexPath, fragPath} {
		use();
		glUniform1i(glGetUniformLocation(ID, "screenTexture"), 0);
	}
	void render(const Scene& scene, const Camera& camera) override {
		use();
		//unsigned int grassDiffuse = textureFromFile("images/grass.png");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, scene.FBOColorTexture);
		//glBindTexture(GL_TEXTURE_2D, grassDiffuse);
		glDisable(GL_DEPTH_TEST);
		glBindVertexArray(scene.screenQuadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glEnable(GL_DEPTH_TEST);
	}
};