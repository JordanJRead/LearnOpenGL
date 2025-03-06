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
	std::vector<Model> mModels{};
	std::vector<Model> mTransparentModels{};
	std::vector<PointLight> mPointLights{};
	std::vector<SpotLight> mSpotLights{};
	DirLight mDirLight{};
	CubeMap mSkyBoxCubeMap;

public:
	Scene(int screenWidth, int screenHeight, const std::vector<std::string>& skyBoxFilePaths);
	const CubeMap& getCubeMap() const;

	const DirLight& getDirLight() const;
	void setDirLight(const DirLight& dirLight);

	const std::vector<SpotLight>& getSpotLights()        const;
	void addSpotLight(const MultiColors& colors, const glm::vec3& dir, float cutoffDot, float outerDot,
		const std::vector<float>& vertices, const Transform& transform);

	const std::vector<PointLight>& getPointLights()       const;
	void addPointLight(const MultiColors& colors, const Attenuation& attenuation, const std::vector<float>& vertices,
		const Transform& transform);

	const std::vector<Model>& getModels()            const;
	void addModel           (const std::string& filePath, const Transform& transform);

	const std::vector<Model>& getTransparentModels() const;
	void addTransparentModel(const std::string& filePath, const Transform& transform);

	void sortTransparent(const glm::vec3& cameraPos);
};

#endif