#version 450 core

layout (location = 0) in vec3 position;

uniform mat4 lightTransformation;

void main()
{
    // The lightTransformation matrix shall also include the model transformation.
    // The multiplication is done on the CPU because the matrix is the same for all the primitives
    // rendered in a single drawing operation.
    gl_Position = lightTransformation * vec4(position, 1.0);
}
