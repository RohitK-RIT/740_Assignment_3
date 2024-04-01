#version 430

#define MAX_LIGHTS 10

struct Light {
    vec3 position;
    vec3 color;
};

uniform Light lights[MAX_LIGHTS];
uniform int num_lights;

in vec3 color;
in vec4 normal;
in vec4 lightDirs[MAX_LIGHTS];

out vec4 frag_color;

void main() {
    vec3 lighting = vec3(0.0);
    for (int i = 0; i < num_lights; i++) {
        float intensity = max(dot(normal, lightDirs[i]), 0.0);
        lighting += intensity * lights[i].color;
    }

    frag_color = vec4(lighting * color, 1.0f);
}