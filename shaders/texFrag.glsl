#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    vec2 tex = vec2(1-TexCoord.x, TexCoord.y);
    FragColor = texture(ourTexture, tex);
}