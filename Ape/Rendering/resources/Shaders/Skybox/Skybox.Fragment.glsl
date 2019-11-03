#version 450 core

in vec3 textureCoordinates;

out vec4 fragmentColor;

uniform samplerCube skybox;

void main()
{
    // Without negating the Z coordinate the skybox looks as if it where rotated 180 degrees around
    // the Y axis (so that the front face becomes the back face and vice versa).
    vec3 coords = vec3(textureCoordinates.x, textureCoordinates.y, -textureCoordinates.z);

    fragmentColor = texture(skybox, coords);
}
