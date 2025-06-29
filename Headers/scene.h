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
public:
	Scene(int screenWidth, int screenHeight, const std::vector<std::string>& skyBoxFilePaths);
	const CubeMap& getSkyBoxCubeMap() const { return mSkyBoxCubeMap; }

	const DirLight&                getDirLight() const { return mDirLight; }
	const std::vector<SpotLight>&  getSpotLights()   const { return mSpotLights; }
	const std::vector<PointLight>& getPointLights() const { return mPointLights; }
	const std::vector<Model>&      getModels() const { return mModels; }
	Model&                         getModel(size_t i) { return mModels[i]; }
	const std::vector<Model>&      getTransparentModels() const { return mTransparentModels; }
	const Model&                   getInstancedModel() const { return mInstancedModel; }

	void setDirLight(const DirLight& dirLight) {
		mDirLight = dirLight;
	}
	void addSpotLight(const glm::vec3& color, const glm::vec3& direction, float cutoffDot, float outerDot, const glm::vec3& position, float ambientScale = 0.2) {
		mSpotLights.emplace_back(color, direction, cutoffDot, outerDot, position, ambientScale);
	}
	void addPointLight(const glm::vec3& color, const Attenuation& attentuation, const glm::vec3& position, float ambientScale = 0.2) {
		mPointLights.emplace_back(color, attentuation, position, ambientScale);
	}
	void addModel(const std::string& filePath, const Transform& transform, bool usesDynamicEnvironmentMapping = false, bool hasBorder = false) {
		mModels.emplace_back(filePath, transform, usesDynamicEnvironmentMapping, hasBorder);
	}
	void addTransparentModel(const std::string& filePath, const Transform& transform, bool usesDynamicEnvironmentMapping = false, bool hasBorder = false) {
		mTransparentModels.emplace_back(filePath, transform, usesDynamicEnvironmentMapping, hasBorder);
	}

	void sortTransparent(const glm::vec3& cameraPos);

private:
	std::vector<Model> mModels{};
	std::vector<Model> mTransparentModels{};
	std::vector<PointLight> mPointLights{};
	std::vector<SpotLight> mSpotLights{};
	Model mInstancedModel;
	DirLight mDirLight{};
	CubeMap mSkyBoxCubeMap;
};

#endif