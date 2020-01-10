#version 450 core

in vec3 textureCoordinates;

out vec4 fragmentColor;

uniform samplerCube skybox;

uniform float fogDensity = 0.02;

// Extern, define in separate shader.
vec3 fog(vec3 color, vec3 cameraToFragment);

void main()
{
    vec4 skyboxColor = texture(skybox, textureCoordinates);

    if (fogDensity > 0.0)
    {
        const float skyboxDistance = 100.0;

        const vec3 fragmentPosition = textureCoordinates * skyboxDistance;

        fragmentColor = vec4(fog(skyboxColor.rgb, fragmentPosition), 1.0);
    }
    else
    {
        fragmentColor = skyboxColor;
    }
}
