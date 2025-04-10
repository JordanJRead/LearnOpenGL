#include "skyboxshader.h"
#include "../OpenGL Wrappers/VAO.h"
#include "../OpenGL Wrappers/BUF.h"
#include <string_view>
#include <vector>
#include "../camera.h"
#include "../scene.h"

void SkyBoxShader::setUniformSkyBoxTexture(int unit) const {
	setInt("cubeMap", unit);
}
SkyBoxShader::SkyBoxShader(std::string_view vertPath, std::string_view fragPath) : Shader{ vertPath, fragPath } {
    use();
    setUniformSkyBoxTexture(0);
}