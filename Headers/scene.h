#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "object.h"
#include "pointLight.h"
#include "structs.h"
#include "spotLight.h"

class Scene {
private:
	std::vector<Object> m_objects{};
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

	void addObject(Material material, const std::vector<float>& vertices, const Transform& transform) {
		m_objects.emplace_back(material, vertices, transform);
	}
	const std::vector<Object>& getObjects() const { return m_objects; }
};

#endif