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


// This struct defines the properties of a Directional Light Caster which casts light in a uniform direction like the sun
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform SpotLight light;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);

    // Ambient Lighting Componenent
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient; 

    // Emission Lighting Component
    vec3 emission = vec3(texture(material.emission, TexCoords)) * material.emmisiveness;

    // Diffuse Lighting Component
    vec3 norm = normalize(Normal);

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

    // Specular Lighting Component
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);

    vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;  

    // Attenuation calculation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    //ambient;
    diffuse *= attenuation;
    specular *= attenuation;

    // Smoothing radius
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity =  clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 final = ambient + intensity * (diffuse + specular + emission); 
    FragColor = vec4(final, 1.0);

}