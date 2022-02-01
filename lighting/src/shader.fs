#version 460 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 frag_pos;
in vec3 normal;

out vec4 frag_color;

uniform vec3 view_pos;

uniform Material material;
uniform Light light;

void main()
{
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light.position - frag_pos);
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);

    vec3 ambient = light.ambient * material.ambient;

    float diff = max(dot(norm, light_dir), 0.0f);
    vec3 diffuse = light.specular * (diff *  material.diffuse);

    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;

    frag_color = vec4(result, 1.0f);
}