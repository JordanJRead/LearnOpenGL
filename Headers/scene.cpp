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
	, mInstancedModel{ "Objects/Cube/cube.obj", {} }
{
}

void Scene::sortTransparent(const glm::vec3& cameraPos) {
	auto compare = [cameraPos](const Model& model1, const Model& model2)->bool {
		return (glm::length(model1.mTransform.getPosition() - cameraPos)) > (glm::length(model2.mTransform.getPosition() - cameraPos));
	};
	std::sort(mTransparentModels.begin(), mTransparentModels.end(), compare);
}