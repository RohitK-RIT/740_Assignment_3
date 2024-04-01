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

uniform float ambient_coefficient;
uniform float diffuse_coefficient;
uniform float specular_coefficient;

in vec3 world_pos;
in vec3 world_normal;

out vec4 frag_color;

void main() {
    vec3 color = vec3(0.0f); // Initialize color to black
    
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

    frag_color = vec4(color, 1.0f);
}