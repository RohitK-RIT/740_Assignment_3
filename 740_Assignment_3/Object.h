#pragma once
#include "Transform.h"

class Object
{
public:
	Transform transform;

protected:
	void create(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f));
};
