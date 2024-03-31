#include "Object.h"

void Object::create(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
{
	transform.position = position;
	transform.rotation = rotation;
	transform.scale = scale;
}
