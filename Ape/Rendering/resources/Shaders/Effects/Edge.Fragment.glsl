#version 450 core

in vec2 textureCoords;

out vec4 fragmentColor;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset, +offset), vec2(0.0, offset),  vec2(+offset, +offset),
        vec2(-offset, 0.0),     vec2(0.0, 0.0),     vec2(+offset, 0.0),
        vec2(-offset, -offset), vec2(0.0, -offset), vec2(+offset, -offset));

    float kernel[9] = float[](
        -1, -1, -1,
        -1, +7, -1,
        -1, -1, -1);

    vec3 color = vec3(0.0);

    for (int i = 0; i < 9; ++i)
    {
        vec2 samplingCoords = textureCoords + offsets[i];

        vec3 sampledColor = vec3(texture(screenTexture, samplingCoords));

        color += sampledColor * kernel[i];
    }

    fragmentColor = vec4(color, 1.0);
}
