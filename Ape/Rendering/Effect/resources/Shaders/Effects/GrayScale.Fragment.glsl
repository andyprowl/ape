#version 450 core

in vec2 textureCoords;

out vec4 fragmentColor;

uniform sampler2D screenTexture;

void main()
{ 
    vec4 textureColor = texture(screenTexture, textureCoords);

    float average = 0.2126 * textureColor.r + 0.7152 * textureColor.g + 0.0722 * textureColor.b;

    fragmentColor = vec4(average, average, average, 1.0);
}
