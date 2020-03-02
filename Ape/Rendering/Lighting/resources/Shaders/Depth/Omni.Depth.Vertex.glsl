#version 450 core

layout (location = 0) in vec3 positionAttribute;

out vec4 worldPosition;

uniform mat4 lightTransformation;

uniform mat4 worldTransformation;

void main()
{
    vec4 modelPosition = vec4(positionAttribute, 1.0);

    worldPosition = worldTransformation * modelPosition;

    // The lightTransformation matrix shall also include the model transformation.
    // The multiplication is done on the CPU because the matrix is the same for all the primitives
    // rendered in a single drawing operation.
    gl_Position = lightTransformation * modelPosition;
}
