#include "PointLight.h"

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut_std.h>

#include "Camera.h"

PointLight::PointLight()
{
	model_mat = mat4(1.0f);
}

PointLight::~PointLight() {}

void PointLight::create(const vec3& position, const vec3& color, vec3 ambient_color, vec3 diffuse_color, vec3 specular_color, float falloff_power, const float intensity,
                        const char* v_shader_file, const char* f_shader_file)
{
	Object::create(position);

	this->ambient_color = ambient_color;
	this->diffuse_color = diffuse_color;
	this->specular_color = specular_color;
	this->falloff_power = falloff_power;

	v_shader.create(v_shader_file, GL_VERTEX_SHADER);
	f_shader.create(f_shader_file, GL_FRAGMENT_SHADER);
	shader_program.create();
	shader_program.link(v_shader);
	shader_program.link(f_shader);

	v_shader.destroy();
	f_shader.destroy();

	this->color = color;
	this->intensity = intensity;
}

void PointLight::draw(mat4 view_mat, mat4 proj_mat, bool is_selected)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_FILL);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glUseProgram(shader_program.id);
	mat4 m = translate(mat4(1.0), transform.position);
	model_mat = scale(m, vec3(0.3f));
	shader_program.setMatrix4fv("modelMat", 1, value_ptr(model_mat));
	shader_program.setMatrix4fv("viewMat", 1, value_ptr(view_mat));
	shader_program.setMatrix4fv("projMat", 1, value_ptr(proj_mat));
	glColor3f(1.0f, 0.0f, 0.0f);
	if (is_selected)
		glutSolidSphere(0.5f, 20, 20);
	else
		glutWireSphere(0.5f, 20, 20);

	glPopMatrix();
	glDisable(GL_POLYGON_OFFSET_FILL);

	glPopMatrix();
}
