#version 330 core

in vec3 vertexColor;

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D texSampler1;

uniform sampler2D texSampler2;

uniform float weight;

uniform float colorWeight;

void main()
{
    vec2 invertedCoord = vec2(1.0 - texCoord.x, texCoord.y);

    vec4 texColor = mix(
        texture(texSampler1, texCoord),
        texture(texSampler2, invertedCoord),
        weight);

    fragColor = mix(texColor, vec4(vertexColor, 1.0), colorWeight);
}