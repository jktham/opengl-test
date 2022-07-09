#version 460 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_offset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{
    gl_Position = projection * view * model * vec4(a_pos + a_offset, 1.0f);
}