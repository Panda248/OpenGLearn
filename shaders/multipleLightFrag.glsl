#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D emission;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    float emmisiveness;
};

uniform Material material;

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
uniform SpotLight spotLight;

struct DirectionLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionLight directionLight;

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

#define NR_POINT_LIGHTS 1

uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcDirLight(DirectionLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() 
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 final = CalcDirLight(directionLight, norm, viewDir);

    final += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    for (int i = 0; i < NR_POINT_LIGHTS; i++) 
    {
        final += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    FragColor = vec4(final, 1.0);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    float theta = dot(viewDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;

    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);


    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
        
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(lightDir, normal), 0.0f);

    vec3 diffuse = light.diffuse * texture(material.diffuse, TexCoords).rgb * diff;

    vec3 reflectDir = reflect (-lightDir, normal);

    float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
    vec3 specular = light.specular * texture(material.specular, TexCoords).rgb * spec;

    // attenuation
    float distance = length(light.position - fragPos);

    float anttenuation = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * (distance * distance)));

    ambient *= anttenuation;
    diffuse *= anttenuation;
    specular *= anttenuation;

    return ambient + intensity * (diffuse + specular);

}

vec3 CalcDirLight(DirectionLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    float diff = max(dot(lightDir, normal), 0.0f);
    
    vec3 diffuse = light.diffuse * texture(material.diffuse, TexCoords).rgb * diff;

    float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);

    vec3 specular = light.specular * texture(material.specular, TexCoords).rgb * spec;

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
        
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(lightDir, normal), 0.0f);

    vec3 diffuse = light.diffuse * texture(material.diffuse, TexCoords).rgb * diff;

    vec3 reflectDir = reflect (-lightDir, normal);

    float spec = pow(max(dot(reflectDir, viewDir), 0.0f), material.shininess);
    vec3 specular = light.specular * texture(material.specular, TexCoords).rgb * spec;

    // attenuation
    float distance = length(light.position - fragPos);

    float intensity = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * distance * distance)) ;

    diffuse *= intensity;
    specular *= intensity;

    return ambient + diffuse + specular;
}