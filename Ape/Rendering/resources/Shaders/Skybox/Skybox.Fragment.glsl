#version 450 core

in vec3 textureCoordinates;

out vec4 fragmentColor;

uniform samplerCube skybox;

void main()
{
    fragmentColor = texture(skybox, textureCoordinates);
}
