#ifndef GAMMASHADER_H
#define GAMMASHADER_H

#include "shader.h"
#include <string>
#include "../OpenGL Wrappers/TEX.h"
#include "../OpenGL Wrappers/VAO.h"
#include "glad/glad.h"

class GammaCorrectionShader : public Shader {
public:
	GammaCorrectionShader(const std::string& vertPath, const std::string& fragPath, VAO& screenQuadVAO) : Shader{ vertPath, fragPath }, mScreenQuadVAO{ screenQuadVAO } {
		this->use();
		setInt("image", 0);
	}

	// Outputs a gamma corrected version of tex to the current framebuffer
	void Correct(const TEX& tex) {
        use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        glBindVertexArray(mScreenQuadVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
	}

private:
	VAO& mScreenQuadVAO;
};

#endif