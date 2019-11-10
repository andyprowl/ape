#version 450 core

struct PointLightView
{

    mat4 right;

    mat4 left;

    mat4 top;

    mat4 bottom;

    mat4 front;

    mat4 back;

};

layout (location = 0) in vec3 positionAttribute;

void main()
{
    gl_Position = vec4(positionAttribute, 1.0);
}
