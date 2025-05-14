#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform bool useTexture;
uniform vec3 objectColor;
uniform sampler2D texture_diffuse1;

void main()
{
    // Normalize input vectors
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Ambient lighting component
    vec3 ambient = 0.1 * lightColor;

    // Increase Diffuse Lighting
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = 0.3 * spec * lightColor;

    // Combine all lighting components
    vec3 lighting = ambient + diffuse + specular;

    // Determine the base color from texture or solid color
    vec3 baseColor =  texture(texture_diffuse1, TexCoords).rgb;
   
 
    // Apply lighting to base color
    vec3 finalColor = lighting * baseColor;
    FragColor = vec4(finalColor, 1.0);
}