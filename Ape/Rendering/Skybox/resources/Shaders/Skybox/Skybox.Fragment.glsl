#version 450 core

in vec3 textureCoordinates;

out vec4 fragmentColor;

uniform samplerCube skybox;

uniform float cameraHeight;

uniform float fogDensity = 0.01;

vec4 fog(vec4 color)
{
    const vec4 fogColor = vec4(0.5, 0.5, 0.5, 1.0);

    const float cameraHeightFactor = pow(0.8, cameraHeight);

    const float skyHeight = 200.0;

    const float fragmentHeightFactor = pow(0.9, (textureCoordinates.y + 0.1) * skyHeight);

    float fogFactor = 
        1.0 /
        exp(fogDensity * skyHeight * (cameraHeightFactor + fragmentHeightFactor));

    fogFactor = clamp(fogFactor, 0.0, 1.0);

    float grayTone = length(color.xyz) / 3.0;

    vec4 grayedOutColor = vec4(grayTone, grayTone, grayTone, 1.0);

    grayedOutColor = mix(color, grayedOutColor, pow(0.95, cameraHeight));

    return mix(fogColor, grayedOutColor, fogFactor);
}

void main()
{
    vec4 skyboxColor = texture(skybox, textureCoordinates);

    if (fogDensity > 0.0)
    {
        fragmentColor = fog(skyboxColor);
    }
    else
    {
        fragmentColor = skyboxColor;
    }
}
