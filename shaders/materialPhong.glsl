#version 330 core

out vec4 FragColor;

// This struct defines the colors for a model's ambient, diffuse, and specular lights as well as shininess
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
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

void main()
{

    // Ambient Lighting Componenent
    vec3 ambient = material.ambient * light.ambient; 

    // Diffuse Lighting Component
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos); 
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = diff * material.diffuse * light.diffuse;

    // Specular Lighting Component
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);

    vec3 specular = material.specular * spec * light.specular;  


    vec3 final = ambient + diffuse + specular; 
    FragColor = vec4(final, 1.0);
}