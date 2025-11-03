#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;

void main()
{
    // This lighting implementation uses the "Phong Lighting" method which is comprised of: Ambient, Diffuse, and Specular lighting.
    // In comparison to "Gourad Shading", the lighting is computed in the fragment shader. This makes the calculations more expensive,
    // but increases fidelity of shading. More fragments than vertices means more calculated points and less interpolation

    /*
    Ambient Lighting emulates global illumination by applying a flat light value throughout the model.
    Global illumination is a result of light constantly reflecting within an environment, reaching spots that normally wouldn't
    have been reached. 
    */

    // Ambient Lighting Componenent
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor; // calculated ambient light color

    /*
    Diffuse Lighting is what usually comes to mind with lighting. 
    Brightness from diffuse lighting is dependent on the alignment of the fragment with the light rays.
    Imagine a light ray reflects on the surface, that reflection has an angle to the fragment's normal vector. 
    The closer that angle is to 0, the brighter the surface should be. This can be calculated with the dot product.
    */

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos); // order matters here. if you flipped, your dot product would be inverted.
    // note these are normalized. What happens if they aren't normalized?

    float diff = max(dot(norm, lightDir), 0.0); // what would negative light look like?
    vec3 diffuse = diff * lightColor;

    /*
    Specular lighting is similar to Diffuse, but also takes into account the view direction (where the player is looking).
    Specular lighting is supposed to mimic highlights. So why do we consider view position? Think why highlights exist.
    Highlights are spots where the light is reflected directly into your eye.
    To mimic this, we reflect the lightray around the normal vector, then dot with the view direction.
    */
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128); // What does the 32 mean? Mess with it (answer, its shininess)
    vec3 specular = specularStrength * spec * lightColor;  


    vec3 final = objectColor * (ambient + diffuse + specular); // why are we summing the lighting components? What happens if we multiply?
    FragColor = vec4(final, 1.0);
}