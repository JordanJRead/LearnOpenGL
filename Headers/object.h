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
#include "vertexarraywrapper.h"

// VAO, model, material
class Object {
private:
	Texture m_diffusionMap;
	Texture m_specularMap;
	Texture m_emissionMap;
	VertexArrayWrapper vertexArrayWrapper;
	float shininess;
public:
	glm::mat4 model;
	unsigned int VBO; // temporary for light source
	int vertexCount;
	Object(Material material, const std::vector<float>& vertices, const Transform& transform)
		: m_diffusionMap{ material.diffuseMapPath, 0 }
		, m_specularMap{ material.specularMapPath, 1 }
		, m_emissionMap{ material.emissionMapPath, 2 }
		, shininess{ material.shininess }
		, vertexArrayWrapper{ vertices }
	{
		model = glm::mat4(1.0);
		model = glm::translate(model, transform.pos);
		model = glm::rotate(model, transform.rotation.y, { 0, 1, 0 });
		model = glm::rotate(model, transform.rotation.x, { 1, 0, 0 });
		model = glm::rotate(model, transform.rotation.z, { 0, 0, 1 });
		model = glm::scale(model, transform.scale);

		vertexCount = vertices.size() / 8;
	}

	void load(const LightingShader& lightingShader) {
		lightingShader.use();
		lightingShader.setUniformModel(model);
		glBindVertexArray(m_VAO);
		m_diffusionMap.use();
		m_specularMap.use();
		m_emissionMap.use();
	}

	void updateTransform(const Transform& transform) {
		model = glm::mat4(1.0);
		model = glm::translate(model, transform.pos);
		model = glm::rotate(model, transform.rotation.z, { 0, 0, 1 });
		model = glm::rotate(model, transform.rotation.x, { 1, 0, 0 });
		model = glm::rotate(model, transform.rotation.y, { 0, 1, 0 });
		model = glm::scale(model, transform.scale);
	}

	~Object() {
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &VBO);
	}
};

#endif