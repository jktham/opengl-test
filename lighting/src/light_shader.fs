#version 460 core

in vec2 tex_coord;

out vec4 frag_color;

uniform sampler2D texture1;

uniform vec3 object_color;
uniform vec3 light_color;

void main()
{
    frag_color = vec4(1.0f);
}