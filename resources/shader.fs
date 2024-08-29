#version 330 core
in vec2 texture_coordinates;
in vec3 normal;
in vec3 fragment_position;
uniform sampler2D texture0;

float ambient_strength = 0.4;
float diffuse_strength = 0.5;
vec3 ambient_color = vec3(0.9, 0.9, 0.9);
vec3 light_color = vec3(0.9, 0.9, 0.9);
vec3 light_position = vec3(2.0, 1.0, 2.0);

void main() {
    vec3 normalized_normal = normalize(normal);
    vec3 light_direction = normalize(light_position - fragment_position);
    float difference = max(dot(normalized_normal, light_direction), 0.0);
    vec3 diffuse = difference * light_color;
    vec3 result = (ambient_strength * ambient_color) + (diffuse * diffuse_strength);
    
    gl_FragColor = vec4(result, 1.0) * texture(texture0, texture_coordinates);
};

