#version 330 core
out vec4 FragColor;
  
//in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D u_Texture;
uniform float u_floor;

void main()
{
    FragColor = texture(u_Texture, TexCoord);
    if(FragColor.x + FragColor.y + FragColor.z + FragColor.w < u_floor * 4) {
        FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
}