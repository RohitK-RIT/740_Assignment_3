﻿#version 430

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;

out vec3 world_pos;
out vec3 world_normal;

void main() {
    world_pos = (modelMat * vec4 (vertex_position, 1.0f)).xyz;
    world_normal = normalize (modelMat * vec4(vertex_normal, 0.0f)).xyz;

    gl_Position = projMat * viewMat * modelMat * vec4(vertex_position, 1.0f);
}