// #include is a custom extension
#include "Vertex.glsi"

struct Material
{

    vec3 ambient;

    sampler2D diffuse;

    sampler2D specular;

    float shininess;

};

struct Attenuation
{

    float constant;

    float linear;

    float quadratic;

};

struct LightColor
{

    vec3 ambient;

    vec3 diffuse;

    vec3 specular;

};

struct PointLight
{

    vec3 position;

    LightColor color;

    Attenuation attenuation;

};

struct SpotLight
{

    vec3 position;

    vec3 direction;

    float cutoffCosine;

    LightColor color;

};

struct DirectionalLight
{

    vec3 direction;

    LightColor color;

};

struct Lighting
{

    #define MAX_NUM_OF_POINT_LIGHTS 8

    #define MAX_NUM_OF_SPOT_LIGHTS 8

    #define MAX_NUM_OF_DIRECTIONAL_LIGHTS 8

    PointLight point[MAX_NUM_OF_POINT_LIGHTS];

    int numOfPointLights;

    SpotLight spot[MAX_NUM_OF_SPOT_LIGHTS];

    int numOfSpotLights;

    DirectionalLight directional[MAX_NUM_OF_DIRECTIONAL_LIGHTS];

    int numOfDirectionalLights;

};

in Vertex vertex;

uniform Material material;

uniform Lighting lighting;

uniform vec3 viewPosition;

vec3 computeAmbientLight(LightColor color)
{
    vec3 diffuseColor = vec3(texture(material.diffuse, vertex.textureCoords));

    return color.ambient * material.ambient * diffuseColor;
}

vec3 computeDiffuseLight(LightColor color, vec3 lightDirection)
{
    float diffusion = max(dot(vertex.normal, lightDirection), 0.0);

    vec3 diffuseColor = vec3(texture(material.diffuse, vertex.textureCoords));

    return color.diffuse * (diffusion * diffuseColor);
}

vec3 computeSpecularLight(LightColor color, vec3 lightDirection)
{
    vec3 viewDirection = normalize(viewPosition - vertex.position);

    vec3 reflectDirection = reflect(-lightDirection, vertex.normal);

    float reflection = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);

    vec3 specularColor = vec3(texture(material.specular, vertex.textureCoords));

    return color.specular * (reflection * specularColor);
}

vec3 computePointLight(PointLight light)
{
    vec3 lightDirection = normalize(light.position - vertex.position);

    vec3 ambientLight = computeAmbientLight(light.color);

    vec3 diffuseLight = computeDiffuseLight(light.color, lightDirection);

    vec3 specularLight = computeDiffuseLight(light.color, lightDirection);

    float sourceDistance = length(light.position - vertex.position);

    float attenuation = 
        1.0 / 
        (light.attenuation.constant + 
         light.attenuation.linear * sourceDistance +
         light.attenuation.quadratic * (sourceDistance * sourceDistance));

    return attenuation * (ambientLight + diffuseLight + specularLight);
}

vec3 computePointLighting()
{
    vec3 color = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < lighting.numOfPointLights; ++i)
    {
        PointLight light = lighting.point[i];

        color += computePointLight(light);
    }

    return color;
}

vec3 computeSpotLight(SpotLight light)
{
    return vec3(0.0, 0.0, 0.0);
}

vec3 computeSpotLighting()
{
    vec3 color = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < lighting.numOfSpotLights; ++i)
    {
        SpotLight light = lighting.spot[i];

        color += computeSpotLight(light);
    }

    return color;
}

vec3 computeDirectionalLight(DirectionalLight light)
{
    vec3 lightDirection = normalize(-light.direction);

    vec3 ambientLight = computeAmbientLight(light.color);

    vec3 diffuseLight = computeDiffuseLight(light.color, lightDirection);

    vec3 specularLight = computeSpecularLight(light.color, lightDirection);

    return (ambientLight + diffuseLight + specularLight);
}

vec3 computeDirectionalLighting()
{
    vec3 color = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < lighting.numOfDirectionalLights; ++i)
    {
        DirectionalLight light = lighting.directional[i];

        color += computeDirectionalLight(light);
    }

    return color;
}

void main()
{

    vec3 pointLighting = computePointLighting();

    vec3 spotLighting = computeSpotLighting();

    vec3 directionalLighting = computeDirectionalLighting();

    gl_FragColor = vec4(pointLighting + directionalLighting + spotLighting, 1.0);
}
