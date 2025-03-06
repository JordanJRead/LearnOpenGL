#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "spotLight.h"
#include "model.h"
#include "pointLight.h"
#include "OpenGL Wrappers/TEX.h"
#include "OpenGL Wrappers/FBO.h"
#include "OpenGL Wrappers/RBO.h"
#include "cubemap.h"

class DirLight;
class MultiColors;
class Transform;
class Attenuation;

class Scene {
private:
	void createFramebuffer(int screenWidth, int screenHeight);
	std::vector<Model> mModels{};
	std::vector<Model> mTransparentModels{};
	std::vector<PointLight> mPointLights{};
	std::vector<SpotLight> mSpotLights{};
	DirLight mDirLight{};
	RBO mFBODepthStencilRBO;
	CubeMap mSkyBoxCubeMap;

public:
	FBO mFBO;
	TEX mFBOColorTex;
	const CubeMap& getCubeMap() const;
	Scene(int screenWidth, int screenHeight, const std::vector<std::string>& skyBoxFilePaths);
	void setDirLight(const DirLight& dirLight);

	//template <class... SpotLight>
	//void addSpotLight(SpotLight&&... spotLight) {
	//	// decltype?
	//	m_spotLights.emplace_back(std::forward<SpotLight>(spotLight)...);
	//}

	void addSpotLight(const MultiColors& colors, const glm::vec3& dir, float cutoffDot, float outerDot,
		const std::vector<float>& vertices, const Transform& transform);

	void addPointLight(const MultiColors& colors, const Attenuation& attenuation, const std::vector<float>& vertices,
		const Transform& transform);

	void addModel           (const std::string& filePath, const Transform& transform);
	void addTransparentModel(const std::string& filePath, const Transform& transform);

	const DirLight& getDirLight() const;
	const std::vector<SpotLight>&  getSpotLights()        const;
	const std::vector<PointLight>& getPointLights()       const;
	const std::vector<Model>&      getModels()            const;
	const std::vector<Model>&      getTransparentModels() const;
	void sortTransparent(const glm::vec3& cameraPos);
	void updateFramebufferSize(int width, int height);
};

#endif