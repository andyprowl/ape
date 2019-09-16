#version 330 core

struct Vertex
{

    vec3 normal;

    vec3 position;

    vec2 textureCoords;

};

struct Transform
{

    mat4 model;

    mat4 camera;

    mat3 normal;

};

layout (location = 0) in vec3 aPosition;

layout (location = 1) in vec3 aNormal;

layout (location = 2) in vec2 aTextureCoords;

out Vertex vertex;

uniform Transform transform;

void main()
{
    gl_Position = transform.camera * transform.model * vec4(aPosition, 1.0);

    vertex.position = vec3(transform.model * vec4(aPosition, 1.0));

    vertex.normal = normalize(transform.normal * aNormal);

    vertex.textureCoords = vec2(1.0 - aTextureCoords.x, aTextureCoords.y);
}
