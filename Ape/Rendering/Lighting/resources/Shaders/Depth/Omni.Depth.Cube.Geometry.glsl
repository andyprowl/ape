#version 450 core

layout (triangles) in;

layout (triangle_strip, max_vertices = 18) out;

struct OmnidirectionalTransformation
{

    mat4 right;

    mat4 left;

    mat4 top;

    mat4 bottom;

    mat4 front;

    mat4 back;

};

out vec4 worldPosition;

uniform mat4 worldTransformation;

uniform OmnidirectionalTransformation lightTransformation;

void emitVerticesInLightSpace(int layerIndex, mat4 transformation)
{
    gl_Layer = layerIndex;

    for (int i = 0; i < 3; ++i)
    {
        vec4 rawPosition = gl_in[i].gl_Position;

        worldPosition = worldTransformation * rawPosition;

        gl_Position = transformation * rawPosition;

        EmitVertex();
    }

    EndPrimitive();
}

void main()
{
    emitVerticesInLightSpace(0, lightTransformation.right);

    emitVerticesInLightSpace(1, lightTransformation.left);

    emitVerticesInLightSpace(2, lightTransformation.top);

    emitVerticesInLightSpace(3, lightTransformation.bottom);

    emitVerticesInLightSpace(4, lightTransformation.front);

    emitVerticesInLightSpace(5, lightTransformation.back);
}
