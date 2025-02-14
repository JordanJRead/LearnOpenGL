#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "pointLight.h"
#include "structs.h"
#include "spotLight.h"
#include "model.h"

class Scene {
private:
	std::vector<Model> mModels{};
	std::vector<PointLight> mPointLights{};
	std::vector<SpotLight> mSpotLights{};
	DirLight mDirLight{};
	std::vector<glm::vec3> mGrassPositions{};
	unsigned int RBO;

public:
	unsigned int grassDiffuse;
	unsigned int grassSpecular;
	unsigned int FBO;
	unsigned int FBOColorTexture;
	unsigned int screenQuadVAO;
	unsigned int screenQuadVBO;
	void initFBO();
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

	void addModel(const std::string& filePath, const Transform& transform, bool hasBorder = false);
	void addGrassPosition(const glm::vec3& pos);

	const DirLight& getDirLight() const;
	const std::vector<SpotLight>&  getSpotLights()     const;
	const std::vector<PointLight>& getPointLights()    const;
	const std::vector<Model>&      getModels()         const;
	const std::vector <glm::vec3>& getGrassPositions() const;
	void sortTransparent(const glm::vec3& cameraPos);
	~Scene() {
		glDeleteTextures(1, &grassDiffuse);
		glDeleteTextures(1, &grassSpecular);

		glDeleteVertexArrays(1, &screenQuadVAO);
		glDeleteBuffers(1, &screenQuadVBO);
		glDeleteFramebuffers(1, &FBO);
		glDeleteTextures(1, &FBOColorTexture);
		glDeleteRenderbuffers(1, &RBO);
	}
};

#endif