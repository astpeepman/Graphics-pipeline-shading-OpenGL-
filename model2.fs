#version 330 core
out vec4 FragColor;



in VS_OUT {
   vec3 FragPos;
   vec2 TexCoords;
   mat3 TBN;
} fs_in;


struct Material {
    sampler2D diffuseMap1;
    sampler2D specularMap1;
    sampler2D normalMap1;
    float shininess;
}; 


struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


#define NR_POINT_LIGHTS 1

uniform vec3 viewPos;
uniform DirLight dirLight;

uniform PointLight pointLights;
uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 color);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color);

vec3 TangentViewPos;
vec3 TangentFragPos;
vec3 TangentLightPos;


void main()
{           

    vec3 normal = texture(material.normalMap1, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0); 

    vec3 color = texture(material.diffuseMap1, fs_in.TexCoords).rgb;

    TangentViewPos= fs_in.TBN * viewPos;
    TangentFragPos= fs_in.TBN * fs_in.FragPos;

    vec3 viewDir =normalize(TangentViewPos - TangentFragPos);

    vec3 result=CalcDirLight(dirLight, normal, viewDir, color);

    result += CalcPointLight(pointLights, normal, fs_in.FragPos, viewDir, color); 

    FragColor = vec4(result, 1.0);
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 color)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * color;
    vec3 diffuse = light.diffuse * diff * color;
    vec3 specular = light.specular * spec * vec3(texture(material.specularMap1, fs_in.TexCoords));
    return (ambient + diffuse + specular);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color)
{
    vec3 TangentLightPos=fs_in.TBN * light.position;
    
    vec3 lightDir =normalize(TangentLightPos - TangentFragPos);

    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse =light.diffuse*diff * color;

    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    vec3 ambient = light.ambient * color;
    vec3 specular = light.specular * spec * vec3(texture(material.specularMap1, fs_in.TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}


