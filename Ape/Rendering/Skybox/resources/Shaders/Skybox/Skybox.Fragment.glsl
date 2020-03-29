#version 460 core

struct Camera
{

    vec3 position;

};

uniform Camera camera;

in vec3 textureCoordinates;

out vec4 fragmentColor;

uniform samplerCube skybox;

// Extern, defined in separate shader.
vec3 applyFog(const vec3 color, const vec3 cameraToFragment);

void main()
{
    vec4 skyboxColor = texture(skybox, textureCoordinates);

    const float skyboxDistance = 100.0;

    const vec3 cameraToFragment = normalize(textureCoordinates) * skyboxDistance;

    fragmentColor = vec4(applyFog(skyboxColor.rgb, cameraToFragment), 1.0);
}
