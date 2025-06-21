#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "glm/glm.hpp"

class Transform {
public:
	Transform(glm::vec3 position = { 0, 0, 0 }, glm::vec3 scale = { 1, 1, 1 }, glm::vec3 rotation = { 0, 0, 0 });
	const glm::vec3& getPosition()    const { return mPosition; }
	const glm::vec3& getScale()       const { return mScale; }
	const glm::vec3& getRotation()    const { return mRotation; }
	const glm::mat4& getModelMatrix() const { return mModelMatrix; }
	void incrementPosition(const glm::vec3& inc);

private:
	glm::vec3 mPosition;
	glm::vec3 mScale;
	glm::vec3 mRotation;
	glm::mat4 mModelMatrix;

private:
	void calculateModelMatrix();
};

#endif