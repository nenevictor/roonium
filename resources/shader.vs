#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texture_coordinates;
uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;
out vec2 texture_coordinates;
out vec3 normal;
out vec3 fragment_position;

void main() {
    texture_coordinates = vec2(a_texture_coordinates.x, 1.0-a_texture_coordinates.y);
    normal = mat3(transpose(inverse(u_model))) * a_normal;
    fragment_position = vec3(u_model * vec4(a_position, 1.0f));
    gl_Position = u_projection * u_view * u_model *  vec4(a_position, 1.0);
};

