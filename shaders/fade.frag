#version 330 core

uniform float u_opacity;

out vec4 FragColor;

void main() {
    FragColor = vec4(0.0, 0.0, 0.0, u_opacity);
}