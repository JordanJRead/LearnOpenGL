#include <vector>
#include "pointLight.h"
#include "structs.h"
#include "spotLight.h"
#include "model.h"
#include "scene.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

extern int g_width; // ?
extern int g_height;

void Scene::initFBO() {
	// Vertex Info
	float quadVertices[] = {
		// positions  // texCoords
		-1.0f, 1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f, 1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f, 1.0f,   1.0f, 1.0f
	};

	glGenVertexArrays(1, &screenQuadVAO);
	glBindVertexArray(screenQuadVAO);

	glGenBuffers(1, &screenQuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, std::size(quadVertices) * sizeof(float), quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// Framebuffer
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Color Attatchment
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &FBOColorTexture);
	glBindTexture(GL_TEXTURE_2D, FBOColorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g_width, g_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOColorTexture, 0);
	// todo make g_width and g_height change with dim. change
	// todo clear fbo buffer?

	// Depth / Stencil
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, g_width, g_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL, GL_RENDERBUFFER, RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "FRAMEBUFFER ERROR\n";
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Scene::Scene() {
	initFBO();
	grassDiffuse = textureFromFile("images/window.png"); // todo memory unsafe (RAII)
	grassSpecular = textureFromFile("images/white.png");
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

void Scene::addModel(const std::string& filePath, const Transform& transform, bool hasBorder) {
	mModels.emplace_back(filePath, transform, hasBorder);
}
void Scene::addGrassPosition(const glm::vec3& pos) {
	mGrassPositions.push_back(pos);
}

const DirLight&                Scene::getDirLight()       const { return mDirLight; }
const std::vector<SpotLight>&  Scene::getSpotLights()     const { return mSpotLights; }
const std::vector<PointLight>& Scene::getPointLights()    const { return mPointLights; };
const std::vector<Model>&      Scene::getModels()         const { return mModels; }
const std::vector<glm::vec3>&  Scene::getGrassPositions() const { return mGrassPositions; }
void Scene::sortTransparent(const glm::vec3& cameraPos) {
	auto compare = [cameraPos](const glm::vec3& pos1, const glm::vec3& pos2)->bool {
		return (glm::length(pos1 - cameraPos)) > (glm::length(pos2 - cameraPos));
	};
	std::sort(mGrassPositions.begin(), mGrassPositions.end(), compare);
}