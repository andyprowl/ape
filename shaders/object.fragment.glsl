// #include is a custom extension
#include "common.glsi"

struct Material
{

    vec3 ambient;

    vec3 diffuse;

    vec3 specular;

    float shininess;

};

struct Light
{

    vec3 position;
  
    vec3 ambient;

    vec3 diffuse;

    vec3 specular;

};

in Vertex vertex;

out vec4 fragColor;

uniform sampler2D texSampler1;

uniform sampler2D texSampler2;

uniform float textureWeight;

uniform float colorWeight;

uniform Material material;

uniform Light light;

uniform vec3 viewPosition;

vec4 computeSurfaceColor()
{
    vec2 invertedCoord = vec2(1.0 - vertex.textureCoords.x, vertex.textureCoords.y);

    return mix(
        texture(texSampler1, invertedCoord),
        texture(texSampler2, invertedCoord),
        textureWeight);
}

vec3 computeAmbientLight()
{
    return light.ambient * material.ambient;
}

vec3 computeDiffuseLight(vec3 lightDirection)
{
    float diffusion = max(dot(vertex.normal, lightDirection), 0.0);

    return light.diffuse * (diffusion * material.diffuse);
}

vec3 computeSpecularLight(vec3 lightDirection)
{
    vec3 viewDirection = normalize(viewPosition - vertex.position);

    vec3 reflectDirection = reflect(-lightDirection, vertex.normal);

    float reflection = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);

    return light.specular * (reflection * material.specular);
}

vec3 computePhongLightEffect()
{
    vec3 ambientLight = computeAmbientLight();

    vec3 lightDirection = normalize(light.position - vertex.position);

    vec3 diffuseLight = computeDiffuseLight(lightDirection);

    vec3 specularLight = computeSpecularLight(lightDirection);

    return (ambientLight + diffuseLight + specularLight);
}

void main()
{
    vec4 surfaceColor = computeSurfaceColor();

    vec3 lightEffect = computePhongLightEffect();

    fragColor = surfaceColor * vec4(lightEffect, 1.0);
}
