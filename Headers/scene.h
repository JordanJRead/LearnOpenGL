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

	void addSpotLight(const glm::vec3& dir, float cutoffDot, float outerDot, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, const std::vector<float>& vertices, const Transform& transform) {
		m_spotLights.emplace_back(dir, cutoffDot, outerDot, ambient, diffuse, specular, vertices, transform);
	}
	const std::vector<SpotLight>& getSpotLights() const { return m_spotLights; }

	void addPointLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
		float attConst, float attLinear, float attQuad,
		const std::vector<float>& vertices, const Transform& transform) {
		m_pointLights.emplace_back(ambient, diffuse, specular, attConst, attLinear, attQuad, vertices, transform);
	}
	const std::vector<PointLight>& getPointLights() const { return m_pointLights; };

	void addObject(Material material, const std::vector<float>& vertices, const Transform& transform) {
		m_objects.emplace_back(material, vertices, transform);
	}
	const std::vector<Object>& getObjects() const { return m_objects; }
};

#endif