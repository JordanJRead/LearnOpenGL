#include "transform.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
	: mPosition{ position }
	, mScale{ scale }
	, mRotation{ rotation }
{
	calculateModelMatrix();
}

void Transform::calculateModelMatrix() {
	mModelMatrix = glm::mat4(1.0);
	mModelMatrix = glm::translate(mModelMatrix, mPosition);
	mModelMatrix = glm::rotate(mModelMatrix, mRotation.x, { 1, 0, 0 });
	mModelMatrix = glm::rotate(mModelMatrix, mRotation.y, { 0, 1, 0 });
	mModelMatrix = glm::rotate(mModelMatrix, mRotation.z, { 0, 0, 1 });
	mModelMatrix = glm::scale(mModelMatrix, mScale);
}

void Transform::incrementPosition(const glm::vec3& inc) {
	mPosition += inc;
	calculateModelMatrix();
}