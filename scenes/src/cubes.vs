#version 460 core

layout (location = 0) in vec3 a_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

uniform vec3 positions[1000];

void main()
{
    gl_Position = projection * view * model * vec4(a_pos + positions[gl_InstanceID], 1.0f);
}