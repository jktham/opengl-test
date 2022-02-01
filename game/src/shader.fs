#version 460 core

out vec4 frag_color;

in vec2 tex_coord;

uniform sampler2D dirt_texture;

void main()
{
    frag_color = texture(dirt_texture, tex_coord);
}