#version 450 core

layout (location = 0) in vec3 positionAttribute;

uniform mat4 transformation;

void main()
{
    gl_Position = transformation * vec4(positionAttribute, 1.0);
}
