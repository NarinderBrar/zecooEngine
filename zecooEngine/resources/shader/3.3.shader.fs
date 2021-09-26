#version 330 core

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

in vec3 Normal;
in vec3 FragPos;

uniform vec4 color;

uniform vec3 viewPos;
uniform DirLight dirLight;

uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);


void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result;
    result += CalcDirLight(dirLight, norm, viewDir);
    gl_FragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = (light.ambient * vec3(0.2,0.2,0.2));
    vec3 diffuse = (light.diffuse * diff * vec3(color.r,color.g,color.b));
    vec3 specular = (light.specular * spec * vec3(0.2,0.2,0.2));

    return (ambient + diffuse + specular);
}