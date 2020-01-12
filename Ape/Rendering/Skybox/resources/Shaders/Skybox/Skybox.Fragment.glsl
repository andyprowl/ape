#version 450 core

#include "BlinnPhong/BlinnPhong.Types.glsl"

uniform Camera camera;

in vec3 textureCoordinates;

out vec4 fragmentColor;

uniform samplerCube skybox;

// Extern, defined in separate shader.
vec3 fog(const vec3 color, const vec3 cameraToFragment);

void main()
{
    vec4 skyboxColor = texture(skybox, textureCoordinates);

    const float skyboxDistance = 100.0;

    const vec3 cameraToFragment = normalize(textureCoordinates) * skyboxDistance;

    fragmentColor = vec4(fog(skyboxColor.rgb, cameraToFragment), 1.0);
}
