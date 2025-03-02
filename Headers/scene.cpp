#include <vector>
#include "pointLight.h"
#include "structs.h"
#include "spotLight.h"
#include "model.h"
#include "scene.h"
#include "stb_image.h"
#include <iostream>
#include <algorithm>
#include "OpenGL Wrappers/TEX.h"

TEX textureFromFile(std::string_view imagePath) {
	TEX tex;
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	int width, height, channelCount;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imagePath.data(), &width, &height, &channelCount, 0);

	if (data) {
		auto internalFormat{ channelCount == 3 ? GL_RGB : GL_RGBA };
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else {
		std::cerr << "Failed to load texture data " << imagePath << "\n";
	}
	return tex;
}

void Scene::createFramebuffer(int screenWidth, int screenHeight) {
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mFBOColorTex);
	glBindRenderbuffer(GL_RENDERBUFFER, mFBODepthStencilRBO);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFBOColorTex, 0);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight); // TODO

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mFBODepthStencilRBO);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

Scene::Scene(int screenWidth, int screenHeight) {
	createFramebuffer(screenWidth, screenHeight);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mSkyBoxTex);
}

void Scene::updateFramebufferSize(int screenWidth, int screenHeight) {
	createFramebuffer(screenWidth, screenHeight);
}

void Scene::setDirLight(const DirLight& dirLight) {
	mDirLight = dirLight;
}

//template <class... SpotLight>
//void Scene::addSpotLight(SpotLight&&... spotLight) {
//	// decltype?
//	m_spotLights.emplace_back(std::forward<SpotLight>(spotLight)...);
//}

void Scene::addSpotLight(const MultiColors& colors, const glm::vec3& dir, float cutoffDot, float outerDot,
	const std::vector<float>& vertices, const Transform& transform) {
	mSpotLights.emplace_back(colors, dir, cutoffDot, outerDot, vertices, transform);
}

void Scene::addPointLight(const MultiColors& colors, const Attenuation& attenuation, const std::vector<float>& vertices,
	const Transform& transform) {
	mPointLights.emplace_back(colors, attenuation, vertices, transform);
}

void Scene::addModel(const std::string& filePath, const Transform& transform) {
	mModels.emplace_back(filePath, transform);
}
void Scene::addTransparentModel(const std::string& filePath, const Transform& transform) {
	mTransparentModels.emplace_back(filePath, transform);
}

const DirLight&                Scene::getDirLight()          const { return mDirLight; }
const std::vector<SpotLight>&  Scene::getSpotLights()        const { return mSpotLights; }
const std::vector<PointLight>& Scene::getPointLights()       const { return mPointLights; };
const std::vector<Model>&      Scene::getModels()            const { return mModels; }
const std::vector<Model>&      Scene::getTransparentModels() const { return mTransparentModels; }
void Scene::sortTransparent(const glm::vec3& cameraPos) {
	auto compare = [cameraPos](const Model& model1, const Model& model2)->bool {
		return (glm::length(model1.mTransform.pos - cameraPos)) > (glm::length(model2.mTransform.pos - cameraPos));
	};
	std::sort(mTransparentModels.begin(), mTransparentModels.end(), compare);
}