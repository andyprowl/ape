#version 430 core

layout (location = 0) in vec3 positionAttribute;

layout (location = 1) in vec3 normalAttribute;

layout (location = 2) in vec2 textureCoordsAttribute;

uniform mat4 transformation;

void main()
{
    gl_Position = transformation * vec4(positionAttribute, 1.0);
}
