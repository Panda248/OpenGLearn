#version 330 core

out vec4 FragColor;

// the diffuse field refers to the diffuse map and stores a texture where we can get a vec4 out of uv coords
struct Material {
    sampler2D emission;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    float emmisiveness;
};

uniform Material material;


// This struct defines the position and intensity a light has on each lighting component
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

void main()
{

    // Ambient Lighting Componenent
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient; 

    // Diffuse Lighting Component
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos); 
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

    // Specular Lighting Component
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);

    vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;  

    // Emission Lighting Component
    vec3 emission = vec3(texture(material.emission, TexCoords)) * material.emmisiveness;

    vec3 final = ambient + diffuse + specular + emission; 
    FragColor = vec4(final, 1.0);
}