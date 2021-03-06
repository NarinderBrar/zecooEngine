#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_DIRECTIONAL_LIGHTS 32

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec4 color;
uniform int directionalLightCount;

uniform vec3 viewPos;
uniform DirLight dirLight;

uniform Material material;

uniform int textureSample;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{    
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    float k = mix(0.0,0.0,0.0);
    
    vec3 result;
    result += CalcDirLight(dirLight, norm, viewDir);
    FragColor = vec4(result, 1.0);
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

    if(textureSample == 1)
    {
        // combine results
        vec3 ambient = (light.ambient * vec3(texture(material.diffuse, TexCoords)));
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
        vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

        return (ambient + diffuse + specular);
    }
    else
    {
        vec3 ambient = (light.ambient * vec3(0.2,0.2,0.2));
        vec3 diffuse = (light.diffuse * diff * vec3(color.r,color.g,color.b));
        vec3 specular = (light.specular * spec * vec3(0.2,0.2,0.2));

        return (ambient + diffuse + specular);
    }

    
}