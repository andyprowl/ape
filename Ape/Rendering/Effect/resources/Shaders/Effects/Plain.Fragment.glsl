#version 450 core

in vec2 textureCoords;

out vec4 fragmentColor;

uniform sampler2D screenTexture;

void main()
{ 
    fragmentColor = texture(screenTexture, textureCoords);
}
