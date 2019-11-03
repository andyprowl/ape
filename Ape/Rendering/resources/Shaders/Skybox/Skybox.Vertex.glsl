#version 450 core

layout (location = 0) in vec3 positionAttribute;

out vec3 textureCoordinates;

uniform mat4 transform;

void main()
{
    textureCoordinates = positionAttribute;

    vec4 clipSpacePosition = transform * vec4(positionAttribute, 1.0);

    /**
     * The skybox is drawn after all other objects, but its geometry is centered around the viewer
     * with a side length of 2. The trick below exploits the fact that clip space coordinate are
     * divided by their W component after being emitted by the vertex shader.
     *
     * At the same time, the depth test function is modified so that it passes if the depth value
     * is less than *or equal* to the currently stored one (rather than just less than the
     * currently stored one).
    **/
    gl_Position = clipSpacePosition.xyww;
}
