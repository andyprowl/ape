// #include is a custom extension
#include "common.glsi"

struct Transform
{

    mat4 model;

    mat4 view;

    mat4 proj;

    mat3 normal;

};

layout (location = 0) in vec3 aPos;

layout (location = 1) in vec3 aNormal;

layout (location = 2) in vec3 aColor;

layout (location = 3) in vec2 aTexCoord;

out Vertex vertex;

uniform Transform transform;

void main()
{
    gl_Position = transform.proj * transform.view * transform.model * vec4(aPos, 1.0);

    vertex.position = vec3(transform.model * vec4(aPos, 1.0));

    vertex.normal = normalize(transform.normal * aNormal);

    vertex.color = aColor;

    vertex.textureCoords = aTexCoord;
}
