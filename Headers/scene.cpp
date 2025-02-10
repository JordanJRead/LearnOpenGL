#include <vector>
#include "pointLight.h"
#include "structs.h"
#include "spotLight.h"
#include "model.h"
#include "scene.h"

Scene::Scene() = default;
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

const DirLight&                Scene::getDirLight()    const { return mDirLight; }
const std::vector<SpotLight>&  Scene::getSpotLights()  const { return mSpotLights; }
const std::vector<PointLight>& Scene::getPointLights() const { return mPointLights; };
const std::vector<Model>&      Scene::getModels()      const { return mModels; }