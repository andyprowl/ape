#version 330 core

layout (location = 0) in vec3 aPos;

layout (location = 1) in vec3 aColor;

layout (location = 2) in vec2 aTexCoord;

out vec3 vertexColor;

out vec2 texCoord;

uniform mat4 model;

uniform mat4 view;

uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);

    vertexColor = aColor;

    texCoord = aTexCoord;
}