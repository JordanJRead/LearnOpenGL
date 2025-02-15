#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <glm/glm.hpp>
#include <string>
#include "spotLight.h"
#include "model.h"
#include "pointLight.h"

class DirLight;
class MultiColors;
class Transform;
class Attenuation;

class Scene {
private:
	std::vector<Model> mModels{};
	std::vector<PointLight> mPointLights{};
	std::vector<SpotLight> mSpotLights{};
	DirLight mDirLight{};
	std::vector<glm::vec3> mGrassPositions{};

public:
	unsigned int grassDiffuse;
	unsigned int grassSpecular;
	Scene();
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

	void addModel(const std::string& filePath, const Transform& transform);
	void addGrassPosition(const glm::vec3& pos);

	const DirLight& getDirLight() const;
	const std::vector<SpotLight>&  getSpotLights()     const;
	const std::vector<PointLight>& getPointLights()    const;
	const std::vector<Model>&      getModels()         const;
	const std::vector<glm::vec3>&  getGrassPositions() const;
	void sortTransparent(const glm::vec3& cameraPos);
};

#endif