#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
     // Normalize input vectors
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

     // Ambient lighting component
    vec3 ambient = lightColor * 0.1;
    
    // Increase Diffuse Lighting
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * 2.0;  // Multiply by 2.0 for extra brightness

    // Specular lighting
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128.0);
    vec3 specular = spec * lightColor * 3.0; // Increase brightness by 3.0

   
    // Combine lighting components
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}

