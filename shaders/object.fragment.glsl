#version 330 core

in vec3 vertexNormal;

in vec3 fragmentPosition;

in vec3 vertexColor;

in vec2 textureCoords;

out vec4 fragColor;

uniform sampler2D texSampler1;

uniform sampler2D texSampler2;

uniform float textureWeight;

uniform float colorWeight;

uniform vec3 lightColor;

uniform vec3 lightPosition;

void main()
{
    vec2 invertedCoord = vec2(1.0 - textureCoords.x, textureCoords.y);

    vec4 texColor = mix(
        texture(texSampler1, invertedCoord),
        texture(texSampler2, invertedCoord),
        textureWeight);

    vec4 surfaceColor =  mix(texColor, vec4(vertexColor, 1.0), colorWeight);

    float ambientLightIntensity = 0.1;

    vec4 ambientLight = vec4(lightColor, 1.0) * ambientLightIntensity;

    vec3 lightDirection = normalize(lightPosition - fragmentPosition);

    float diffusion = max(dot(vertexNormal, lightDirection), 0.0);

    vec4 diffuseLight = vec4(lightColor * diffusion, 1.0);

    fragColor = surfaceColor * (ambientLight + diffuseLight);
}