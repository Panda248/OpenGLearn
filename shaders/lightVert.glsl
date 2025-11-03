#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; // this is needed for diffuse lighting

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model *vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal; // forward normal vector to fragment
    // Note: when transforming a model, you cannot apply the same transformations to the normal vector. 
    // For non-uniform transformations, the normal vector would no longer be perpendicular.
    // To fix this, you multiply the vector by whats called the "Normal Matrix". 
    // Formally, this is 'the transpose of the inverse of the upper-left 3x3 part of the model matrix'.
    // Google it for more info. Also, you wouldn't calculate the normal matrix here, you'd do it on the CPU cause its pretty
    // resource intensive.
} 