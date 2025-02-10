#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "pointLight.h"
#include "structs.h"
#include "spotLight.h"
#include "model.h"

class Scene {
private:
	std::vector<Model> m_models{};
	std::vector<PointLight> m_pointLights{};
	std::vector<SpotLight> m_spotLights{};
	DirLight m_dirLight{};

public:
	Scene() = default;
	void setDirLight(const DirLight& dirLight) {
		m_dirLight = dirLight;
	}
	const DirLight& getDirLight() const { return m_dirLight; }

	//void addSpotLight(MultiColors colors, const glm::vec3& dir, float cutoffDot, float outerDot, const std::vector<float>& vertices, const Transform& transform) {
	//	m_spotLights.emplace_back(colors, dir, cutoffDot, outerDot, vertices, transform);
	//}
	//template <class... SpotLight>
	//void addSpotLight(SpotLight&&... spotLight) {
	//	// decltype?
	//	m_spotLights.emplace_back(std::forward<SpotLight>(spotLight)...);
	//}
	void addSpotLight(const MultiColors& colors, const glm::vec3& dir, float cutoffDot, float outerDot, const std::vector<float>& vertices, const Transform& transform) {
		m_spotLights.emplace_back(colors, dir, cutoffDot, outerDot, vertices, transform);
	}
	const std::vector<SpotLight>& getSpotLights() const { return m_spotLights; }

	void addPointLight(const MultiColors& colors, const Attenuation& attenuation, const std::vector<float>& vertices, const Transform& transform) {
		m_pointLights.emplace_back(colors, attenuation, vertices, transform);
	}
	const std::vector<PointLight>& getPointLights() const { return m_pointLights; };

	void addModel(const std::string& filePath, const Transform& transform) {
		m_models.emplace_back(filePath, transform);
	}
	const std::vector<Model>& getModels() const { return m_models; }
};

#endif