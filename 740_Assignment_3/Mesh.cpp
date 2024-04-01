#include "Mesh.h"

#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "PointLight.h"


Mesh::Mesh()
{
	vert_num = tri_num = 0;
	vao = vbo = nbo = ibo = 0;
	model_mat = mat4(1.0f);
}

Mesh::~Mesh()
{
	delete[] vertices;
	delete[] triangles;
	delete[] f_normals;
	delete[] v_normals;
}

void Mesh::computeNormals()
{
	f_normals = new vec3[tri_num];
	v_normals = new vec3[vert_num];

	vec3 a, b, n;

	// Compute face normals
	for (unsigned int i = 0; i < tri_num; i++)
	{
		a = vertices[triangles[i][1]] - vertices[triangles[i][0]];
		b = vertices[triangles[i][2]] - vertices[triangles[i][0]];

		n = cross(a, b);
		f_normals[i] = normalize(n);
	}

	// Compute vertex normals
	for (unsigned int i = 0; i < vert_num; i++)
	{
		v_normals[i] = vec3(0.0f);
	}

	for (unsigned int i = 0; i < tri_num; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			v_normals[triangles[i][j]] += f_normals[i];
		}
	}

	for (unsigned int i = 0; i < vert_num; i++)
	{
		v_normals[i] = normalize(v_normals[i]);
	}
}

void Mesh::prepareVBOandShaders(const char* v_shader_file, const char* f_shader_file)
{
	v_shader.create(v_shader_file, GL_VERTEX_SHADER);
	f_shader.create(f_shader_file, GL_FRAGMENT_SHADER);
	shader_program.create();
	shader_program.link(v_shader);
	shader_program.link(f_shader);

	v_shader.destroy();
	f_shader.destroy();

	// create vbo 
	// generate a new VBO and get the associated ID
	glGenBuffers(1, &vbo);

	// bind VBO in order to use
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// upload data to VBO - data went to GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vert_num, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // clean up
	// delete[] vertices; // commented out, since it's handled by the destructor


	// repeat for normals and indices

	glGenBuffers(1, &nbo);
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vert_num, v_normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // clean up
	// delete[] vnormals; // commented out, since it's handled by the destructor

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uvec3) * tri_num, triangles, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // clean up
	// delete[] triangles; // commented out, since it's handled by the destructor

	// so far, we transferred the position and index data to GPU. There will be no data transfer calls at drawing time

	// Define VAO
	// There are 2 buffer objects (vertices and normals), which should be input "attribute" variables to the vertex shader. 
	// We set up the layout of both of these with vertex array object. 
	// The actual vertices and normal vectors have already been kept on the GPU memory by VBOs. 
	// The purpose of VAO is to have vertices and normal vectors as separate attributes in the vertex shader. 
	// So VAO's attrobites point to these data on the GPU, rather than referring back to any CPU data. 

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); // 0 - the layout id in vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, nbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL); // 1 - the layout id in fragment shader

	// Attributes are disabled by default in OpenGL 4. 
	// We have to explicitly enable each one.
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

void Mesh::create(const char* filename, const vec3& position, const vec3& scale, const char* v_shader_file, const char* f_shader_file)
{
	Object::create(position, vec3(0), scale);

	vector<vec3> ori_vertices;
	vector<uvec3> ori_triangles;

	ifstream fs(filename);

	char c;
	vec3 pos;
	int index[3];
	int vid = 0;

	std::string line;
	while (std::getline(fs, line))
	{
		std::istringstream iss(line);

		iss >> c;

		switch (c)
		{
		case 'v':
			{
				// read a vertex
				iss >> pos.x;
				iss >> pos.y;
				iss >> pos.z;
				ori_vertices.push_back(pos);
				break;
			}
		case 'f':
			{
				// read a triangle's vertex indices
				iss >> index[0];
				iss >> index[1];
				iss >> index[2];
				// NOTE: index in obj files starts from 1
				ori_triangles.emplace_back(index[0] - 1, index[1] - 1, index[2] - 1);
				break;
			}
		default:
			// skip the line
			break;
		}
	}
	fs.close();

	vert_num = ori_vertices.size();
	tri_num = ori_triangles.size();
	vertices = new vec3[vert_num];
	triangles = new uvec3[tri_num];

	// Use arrays to store vertices and triangles, instead of using c++ vectors.
	// This is because we have to use arrays when sending data to GPUs. 
	for (uint i = 0; i < vert_num; i++)
	{
		vertices[i] = ori_vertices[i];
	}
	for (uint i = 0; i < tri_num; i++)
	{
		triangles[i] = ori_triangles[i];
	}

	computeNormals();
	prepareVBOandShaders(v_shader_file, f_shader_file);
}

void Mesh::draw(mat4 viewMat, mat4 projMat, vector<PointLight*> lights, float time)
{
	if (vert_num <= 0 && tri_num <= 0)
		return;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_FILL);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glUseProgram(shader_program.id);
	mat4 m = translate(mat4(1.0), transform.position);
	model_mat = scale(m, transform.scale);
	shader_program.setMatrix4fv("modelMat", 1, value_ptr(model_mat));
	shader_program.setMatrix4fv("viewMat", 1, value_ptr(viewMat));
	shader_program.setMatrix4fv("projMat", 1, value_ptr(projMat));

	shader_program.setFloat("ambient_coefficient", 1.0f);
	shader_program.setFloat("diffuse_coefficient", 1.0f);
	shader_program.setFloat("specular_coefficient", 1.0f);

	int size = lights.size();
	for (int i = 0; i < size; i++)
	{
		std::string lightIndex = std::to_string(i);
		shader_program.setFloat3V(("lights[" + lightIndex + "].position").c_str(), 1, value_ptr(lights[i]->transform.position));
		shader_program.setFloat3V(("lights[" + lightIndex + "].ambient_color").c_str(), 1, value_ptr(lights[i]->ambient_color));
		shader_program.setFloat3V(("lights[" + lightIndex + "].diffuse_color").c_str(), 1, value_ptr(lights[i]->diffuse_color));
		shader_program.setFloat3V(("lights[" + lightIndex + "].specular_color").c_str(), 1, value_ptr(lights[i]->specular_color));
		shader_program.setFloat(("lights[" + lightIndex + "].falloff_power").c_str(), lights[i]->falloff_power);
	}
	shader_program.setInt("num_lights", size);

	shader_program.setFloat("time", time);
	shader_program.setFloat("offset", normal_offset);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, tri_num * 3, GL_UNSIGNED_INT, NULL);

	glPopMatrix();
	glDisable(GL_POLYGON_OFFSET_FILL);

	glPopMatrix();
}
