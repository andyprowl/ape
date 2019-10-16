#version 450 core

layout (location = 0) in vec2 positionAttribute;

layout (location = 1) in vec2 textureCoordsAttribute;

out vec2 textureCoords;

void main()
{
    gl_Position = vec4(positionAttribute.x, positionAttribute.y, 0.0, 1.0);
    
    textureCoords = textureCoordsAttribute;
}
