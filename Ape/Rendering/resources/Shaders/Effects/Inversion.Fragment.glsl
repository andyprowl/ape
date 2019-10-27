#version 450 core

in vec2 textureCoords;

out vec4 fragmentColor;

uniform sampler2D screenTexture;

void main()
{ 
    vec4 textureColor = texture(screenTexture, textureCoords);

    fragmentColor = vec4(vec3(1.0 - textureColor), 1.0);
}
