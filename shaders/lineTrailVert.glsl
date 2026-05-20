#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in float aRadius;
layout (location = 2) in float aOffset;

uniform float u_time;
uniform float u_aspect_ratio;

out vec2 FragPos;

void main()
{
    float t = u_time + aOffset;
    vec2 pos = aPos + vec2(aRadius * cos(t), u_aspect_ratio * aRadius * sin(t));
    gl_Position = vec4(pos, 0.0, 1.0);
    FragPos = gl_Position.xy;
} 