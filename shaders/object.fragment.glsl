#version 330 core

struct Material
{

    vec3 ambientColor;

    vec3 diffuseColor;

    vec3 specularColor;

    float shininess;

};

in vec3 vertexNormal;

in vec3 fragmentPosition;

in vec3 vertexColor;

in vec2 textureCoords;

out vec4 fragColor;

uniform sampler2D texSampler1;

uniform sampler2D texSampler2;

uniform float textureWeight;

uniform float colorWeight;

uniform Material material;

uniform vec3 lightColor;

uniform vec3 lightPosition;

uniform vec3 viewPosition;

vec4 computeSurfaceColor()
{
    vec2 invertedCoord = vec2(1.0 - textureCoords.x, textureCoords.y);

    vec4 texColor = mix(
        texture(texSampler1, invertedCoord),
        texture(texSampler2, invertedCoord),
        textureWeight);

    return mix(texColor, vec4(vertexColor, 1.0), colorWeight);
}

vec3 computeAmbientLight()
{
    return lightColor * material.ambientColor;
}

vec3 computeDiffuseLight(vec3 lightDirection)
{
    float diffusion = max(dot(vertexNormal, lightDirection), 0.0);

    return lightColor * (material.diffuseColor * diffusion);
}

vec3 computeSpecularLight(vec3 lightDirection)
{
    vec3 viewDirection = normalize(viewPosition - fragmentPosition);

    vec3 reflectDirection = reflect(-lightDirection, vertexNormal);

    float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);

    return material.shininess * specularFactor * material.specularColor;
}

vec3 computePhongLightEffect()
{
    vec3 ambientLight = computeAmbientLight();

    vec3 lightDirection = normalize(lightPosition - fragmentPosition);

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