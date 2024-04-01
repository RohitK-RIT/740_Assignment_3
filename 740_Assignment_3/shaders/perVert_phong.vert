#version 430

#define MAX_LIGHTS 10

struct Light {
    vec3 position;

    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;

    float falloff_power;
};

uniform Light lights[MAX_LIGHTS];
uniform int num_lights;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform float ambient_coefficient;
uniform float diffuse_coefficient;
uniform float specular_coefficient;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

out vec3 color;

void main() {
    color = vec3(0.0);// Reset color before calculating
    
    // Transform vertex position and normal to world space
    vec3 world_pos = (modelMat * vec4 (vertex_position, 1.0f)).xyz;
    vec3 world_normal = (modelMat * vec4 (vertex_normal, 0.0f)).xyz;

    for (int i = 0; i < num_lights; i++) {
        // Ambient
        vec3 Ambient = ambient_coefficient * lights[i].ambient_color;

        // Diffuse
        vec3 lightDir = normalize(lights[i].position - world_pos);
        float diff = max(dot(lightDir, world_normal), 0.0);
        vec3 Diffuse = diffuse_coefficient * diff * lights[i].diffuse_color;

        // Specular
        vec3 viewDir = normalize(world_pos);
        vec3 reflectDir = reflect(-lightDir, world_normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), lights[i].falloff_power);
        vec3 Specular = specular_coefficient * spec * lights[i].specular_color;

        // Add all components
        color += Ambient + Diffuse + Specular;
    }

    // Transform vertex position to clip space
    gl_Position = projMat * viewMat * modelMat * vec4(vertex_position, 1.0f);
}