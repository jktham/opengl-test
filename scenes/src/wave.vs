#version 460 core

layout (location = 0) in vec3 a_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{
    float scale_v = 2.0f;
    float scale_h = 1.0f / 20.0f * 3.1415926f;

    float x = a_pos.x * scale_h + time;
    float z = a_pos.z * scale_h + time;
    float y = (sin(x) + sin(z)) * scale_v;

    gl_Position = projection * view * model * vec4(a_pos.x, y, a_pos.z, 1.0f);
}