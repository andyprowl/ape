#version 330 core

layout (location = 0) in vec3 aPos;

layout (location = 1) in vec3 aNormal;

layout (location = 2) in vec3 aColor;

layout (location = 3) in vec2 aTexCoord;

out vec3 fragmentPosition;

out vec3 vertexNormal;

out vec3 vertexColor;

out vec2 textureCoords;

uniform mat4 model;

uniform mat4 view;

uniform mat4 proj;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);

    fragmentPosition = vec3(model * vec4(aPos, 1.0));

    vertexNormal = normalize(mat3(model) * aNormal);

    vertexColor = aColor;

    textureCoords = aTexCoord;
}