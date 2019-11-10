#version 450 core

in vec4 modelPosition;

uniform vec3 lightPosition;

void main()
{
    // TODO: This can be different for each light! This is a hack.
    // The far plane should either be passed a part of the light uniform or the algorithm should be
    // changed to not make use of the far plane.
    const float farPlaneDistance = 100.0;

    float distanceFromLight = length(vec3(modelPosition) - lightPosition);

    float normalizedLightDistance = distanceFromLight / farPlaneDistance;

    gl_FragDepth = normalizedLightDistance;
}
