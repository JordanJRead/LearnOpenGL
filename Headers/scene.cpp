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
#include "cubemap.h"


Scene::Scene(int screenWidth, int screenHeight, const std::vector<std::string>& skyBoxFilePaths)
	: mSkyBoxCubeMap{ skyBoxFilePaths }
	, mInstancedModel{ "Objects/Grass/billboardgrass.obj", {} }
{
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

void Scene::addModel(const std::string& filePath, const Transform& transform, bool usesDynamicEnvironmentMapping, bool hasBorder) {
	mModels.emplace_back(filePath, transform, usesDynamicEnvironmentMapping, hasBorder);
}
void Scene::addTransparentModel(const std::string& filePath, const Transform& transform, bool usesDynamicEnvironmentMapping, bool hasBorder) {
	mTransparentModels.emplace_back(filePath, transform, usesDynamicEnvironmentMapping, hasBorder);
}

const DirLight&                Scene::getDirLight()          const { return mDirLight; }
const std::vector<SpotLight>&  Scene::getSpotLights()        const { return mSpotLights; }
const std::vector<PointLight>& Scene::getPointLights()       const { return mPointLights; };
const std::vector<Model>&      Scene::getModels()            const { return mModels; }
Model& Scene::getModel(size_t i) { return mModels[i]; }
const std::vector<Model>&      Scene::getTransparentModels() const { return mTransparentModels; }
void Scene::sortTransparent(const glm::vec3& cameraPos) {
	auto compare = [cameraPos](const Model& model1, const Model& model2)->bool {
		return (glm::length(model1.mTransform.pos - cameraPos)) > (glm::length(model2.mTransform.pos - cameraPos));
	};
	std::sort(mTransparentModels.begin(), mTransparentModels.end(), compare);
}
const CubeMap& Scene::getCubeMap() const {
	return mSkyBoxCubeMap;
}

const Model& Scene::getInstancedModel() const {
	return mInstancedModel;
}