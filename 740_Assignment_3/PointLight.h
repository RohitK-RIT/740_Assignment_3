#pragma once
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>

#include "Object.h"
#include "ShaderClass.h"
#include "ShaderProgram.h"

class PointLight : public Object
{
public:
	glm::vec3 color;
	glm::vec3 ambient_color;
	glm::vec3 diffuse_color;
	glm::vec3 specular_color;
	float falloff_power;
	float intensity;

	ShaderProgram shader_program;
	ShaderClass v_shader;
	ShaderClass f_shader;

	glm::mat4 model_mat;

	PointLight();
	~PointLight();

	void create(const glm::vec3& position, const glm::vec3& color, glm::vec3 ambient_color, glm::vec3 diffuse_color, glm::vec3 specular_color, float falloff_power, const float intensity,
	            const char* v_shader_file, const char* f_shader_file);
	void draw(glm::mat4 view_mat, glm::mat4 proj_mat, bool is_selected);
};
