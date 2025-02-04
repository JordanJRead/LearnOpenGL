#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include "texture.h"
#include <string>
#include <array>
#include <vector>
#include "structs.h"

// VAO, model, material
class Object {
private:
	unsigned int VAO;
	Texture diffusionMap;
	Texture specularMap;
	Texture emissionMap;
	glm::mat4 model;
public:
	Object(const Material& _material, const std::vector<int>& vertices, const Transform& transform) 
		: diffusionMap{_material}
	{

	}
};

#endif