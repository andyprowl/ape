#version 450 core

layout (location = 0) in vec3 position;

uniform mat4 lightTransformation;

void main()
{
    gl_Position = lightTransformation * vec4(position, 1.0);
}
