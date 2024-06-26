#version 430

#define MAX_LIGHTS 10

struct Light {
    vec3 position;
    vec3 color;
};

uniform Light lights[MAX_LIGHTS];
uniform int num_lights;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform vec3 lightPos;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

out vec3 color;
out vec4 normal;
out vec4 lightDirs[MAX_LIGHTS];

void main() {
    for (int i = 0; i < num_lights; i++) {
        vec4 lpos = viewMat * vec4(lights[i].position, 1.0f);
        vec4 ppos = viewMat * modelMat * vec4 (vertex_position, 1.0f);
        lightDirs[i] = normalize(lpos - ppos);
    }

    normal = normalize (viewMat * modelMat * vec4(vertex_normal, 0.0f));
    color = vec3(1.0, 1.0, 0.0);

    gl_Position = projMat * viewMat * modelMat * vec4(vertex_position, 1.0f);
}




