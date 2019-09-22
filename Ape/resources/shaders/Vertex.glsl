#version 330 core

struct Vertex
{

    vec3 position;

    vec3 normal;

    vec2 textureCoords;

};

struct Camera
{

    vec3 position;

    mat4 transform;

};

struct Model
{

    mat4 transform;

    mat3 normal;

};

layout (location = 0) in vec3 positionAttribute;

layout (location = 1) in vec3 normalAttribute;

layout (location = 2) in vec2 textureCoordsAttribute;

out Vertex vertex;

uniform Model model;

uniform Camera camera;

void main()
{
    gl_Position = camera.transform * model.transform * vec4(positionAttribute, 1.0);

    vertex.position = vec3(model.transform * vec4(positionAttribute, 1.0));

    vertex.normal = normalize(model.normal * normalAttribute);

    vertex.textureCoords = vec2(1.0 - textureCoordsAttribute.x, textureCoordsAttribute.y);
}
