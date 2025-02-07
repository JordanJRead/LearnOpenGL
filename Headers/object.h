#ifndef OBJECT_H
#define OBJECT_H

#include "texture.h"
#include <string>
#include <array>
#include <vector>
#include "structs.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "lightingshader.h"
#include "modelinfo.h"

// VAO, model, material
class Object {
private:
	Texture m_diffusionMap;
	Texture m_specularMap;
	Texture m_emissionMap;
public:
	ModelInfo modelInfo;
	float shininess;
	Object(Material material, const std::vector<float>& vertices, const Transform& transform)
		: m_diffusionMap{ material.diffuseMapPath, 0 }
		, m_specularMap{ material.specularMapPath, 1 }
		, m_emissionMap{ material.emissionMapPath, 2 }
		, shininess{ material.shininess }
		, modelInfo{ vertices, transform }
	{
	}

	void use() const {
		modelInfo.use();
		m_diffusionMap.use();
		m_specularMap.use();
		m_emissionMap.use();
	}
};

#endif