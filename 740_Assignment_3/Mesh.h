#ifndef MESH_H
#define MESH_H

#define GLM_FORCE_RADIANS

#include "ShaderProgram.h"

#include <vector>
#include <set>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/constants.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "Object.h"
#include "PointLight.h"

using namespace std;
using namespace glm;

class Mesh : public Object
{
public:
	/**mesh data: loaded from the file as arrays**/
	uint vert_num, tri_num;

	vec3* vertices;
	uvec3* triangles;
	vec3* f_normals; // normals of triangles, size = # of triangles
	vec3* v_normals; // normal of vertices, computed by averaging the adajecent traingle normals; size = # of vertices

	GLuint vao, vbo, nbo, ibo;
	ShaderProgram shader_program;
	ShaderClass v_shader;
	ShaderClass f_shader;

	mat4 model_mat;

	float normal_offset = 0.0f;

	Mesh();
	~Mesh();

	/* Load the mesh from an '.obj' file. All faces need to be triangles.
	   Attributes like edge lengths and curvatures should be computed when simplifying the mesh.
	*/
	void create(const char* filename, const vec3& position, const vec3& scale, const char* v_shader_file, const char* f_shader_file);

	void draw(mat4 viewMat, mat4 projMat, vector<PointLight*> lights, float time);

private:
	void computeNormals(); // compute both face and vertex normals
	void prepareVBOandShaders(const char* v_shader_file, const char* f_shader_file);
};

#endif
