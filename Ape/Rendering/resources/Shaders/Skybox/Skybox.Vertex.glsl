#version 450 core

struct Transform
{

    mat4 projection;

    mat4 viewRotation;

};

layout (location = 0) in vec3 positionAttribute;

out vec3 textureCoordinates;

uniform Transform transform;

void main()
{
    gl_Position = transform.projection * transform.viewRotation * vec4(positionAttribute, 1.0);
}
