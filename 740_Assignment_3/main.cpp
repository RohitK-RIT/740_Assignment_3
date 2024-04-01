// This OpenGL project demonstrates a simple interactive camera implementation and a few vertex and fragment shaders. 
// Two modes of camera controls (press c to switch between two modes): 
// 1. Focus Mode: Holding ALT and Left Mouse Button (LMB) and moving the mouse will rotate the camera about the LookAt Position
//                Holding ALT and MMB and moving the mouse will pan the camera.
//                Holding ALT and RMB and moving the mouse will zoom the camera.
// 2. First-Person Mode: Pressing W, A, S, or D will move the camera
//                       Holding LMB and moving the mouse will roate the camera.
// Basic shader - demonstrate the basic use of vertex and fragment shader
// Displacement shader - a special fireball visual effects with Perlin noise function
// Toon shading shader - catoonish rendering effects
// Per-vertex shading v.s. per-fragment shading = visual comparison between two types of shading 

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Mesh.h"
#include "PointLight.h"

using namespace std;
using namespace glm;

int win_width = 1024;
int win_height = 512;

Camera cam;

unsigned char key_states[256];

char v_shader_file[] =
	//".\\shaders\\basic.vert";
	//".\\shaders\\displacement.vert"; // vertex displacement shader with perlin noise
	//".\\shaders\\perVert_lambert.vert"; // basic lambert lighting  
	// ".\\shaders\\perFrag_lambert.vert"; // basic lambert lighting with per-fragment implementation
	// ".\\shaders\\toon_shading.vert"; // basic toon shading with per-fragment implementation
	".\\shaders\\perVert_phong.vert"; // basic phong shading with per-vertex implementation

char f_shader_file[] =
	//".\\shaders\\basic.frag";
	// ".\\shaders\\displacement.frag"; // vertex displacement shader with perlin noise
	// ".\\shaders\\perVert_lambert.frag"; // basic lambert shading 
	// ".\\shaders\\perFrag_lambert.frag"; // basic lambert shading with per-fragment implementation
	// ".\\shaders\\toon_shading.frag"; // basic toon shading with per-fragment implementation
	".\\shaders\\perVert_phong.frag"; // basic phong shading with per-vertex implementation

char v_shader_file_s[] =
	".\\shaders\\basic.vert";
//".\\shaders\\displacement.vert"; // vertex displacement shader with perlin noise
//".\\shaders\\perVert_lambert.vert"; // basic lambert lighting  
// ".\\shaders\\perFrag_lambert.vert"; // basic lambert lighting with per-fragment implementation
// ".\\shaders\\toon_shading.vert"; // basic toon shading with per-fragment implementation

char f_shader_file_s[] =
	".\\shaders\\basic.frag";
// ".\\shaders\\displacement.frag"; // vertex displacement shader with perlin noise
// ".\\shaders\\perVert_lambert.frag"; // basic lambert shading 
// ".\\shaders\\perFrag_lambert.frag"; // basic lambert shading with per-fragment implementation
// ".\\shaders\\toon_shading.frag"; // basic toon shading with per-fragment implementation

constexpr char teapot_mesh_file[128] = "Mesh/teapot.obj";

Mesh teapot1, teapot2;
PointLight light1, light2;
PointLight* selected_light = &light1;

// vec3 light1_pos = vec3(3, 3, 3);
float time_elapsed = 0.0f;

void initialization()
{
	cam.set(3.0f, 4.0f, 14.0f, 0.0f, 1.0f, -0.5f, win_width, win_height);

	teapot1.create(teapot_mesh_file, vec3(0.0f, 2.0f, 0.0f), vec3(0.5f), v_shader_file, f_shader_file);
	teapot2.create(teapot_mesh_file, vec3(3.0f, 2.0f, 0.0f), vec3(0.5f), v_shader_file, f_shader_file);

	light1.create(vec3(3.0f, 3.0f, 3.0f), vec3(1.0f, 1.0f, 1.0f),
	              vec3(0.0f, 0.15f, 0.0f), vec3(1.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), 10,
	              0.5f, v_shader_file_s, f_shader_file_s);
	light2.create(vec3(1.0f, 0.0f, -2.0f), vec3(1.0f, 1.0f, 1.0f),
	              vec3(0.0f, 0.0f, 0.15f), vec3(1.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), 10,
	              0.5f, v_shader_file_s, f_shader_file_s);
	
	// add any stuff you want to initialize ...
}

/****** GL callbacks ******/
void initialGL()
{
	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void idle()
{
	// add any stuff to update at runtime ....

	cam.keyOperation(key_states, win_width, win_height);

	glutPostRedisplay();
}

void display()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(0);
	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	cam.drawGrid();
	cam.drawCoordinateOnScreen(win_width, win_height);
	cam.drawCoordinate();

	light1.draw(cam.viewMat, cam.projMat, selected_light == &light1);
	light2.draw(cam.viewMat, cam.projMat, selected_light == &light2);

	vector<PointLight*> lights;
	lights.push_back(&light1);
	lights.push_back(&light2);

	time_elapsed = (float)glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	teapot1.draw(cam.viewMat, cam.projMat, lights, time_elapsed);
	teapot2.draw(cam.viewMat, cam.projMat, lights, time_elapsed);

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	win_width = w;
	win_height = h;
	if (h == 0)
	{
		h = 1;
	}
	cam.setProjectionMatrix(win_width, win_height);
	cam.setViewMatrix();
	glViewport(0, 0, w, h);
}

void mouse(int button, int state, int x, int y)
{
	cam.mouseClick(button, state, x, y, win_width, win_height);
}

void motion(int x, int y)
{
	cam.mouseMotion(x, y, win_width, win_height);
}

void keyup(unsigned char key, int x, int y)
{
	key_states[key] = false;
}

void keyboard(unsigned char key, int x, int y)
{
	key_states[key] = true;
	switch (key)
	{
	case 27:
		exit(0);
	case ' ':
		cam.PrintProperty();
		break;

	// Light movement
	case 'w':
		selected_light->transform.position.z += 0.1f;
		break;
	case 's':
		selected_light->transform.position.z -= 0.1f;
		break;
	case 'a':
		selected_light->transform.position.x -= 0.1f;
		break;
	case 'd':
		selected_light->transform.position.x += 0.1f;
		break;
	case 'u':
		selected_light->transform.position.y += 0.1f;
		break;
	case 'j':
		selected_light->transform.position.y -= 0.1f;
		break;

	// Light selection
	case '1':
		selected_light = &light1;
		break;
	case '2':
		selected_light = &light2;
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(win_width, win_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("VertFrag Shader Example");

	glewInit();
	initialGL();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardUpFunc(keyup);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	initialization();

	glutMainLoop();
	return EXIT_SUCCESS;
}
