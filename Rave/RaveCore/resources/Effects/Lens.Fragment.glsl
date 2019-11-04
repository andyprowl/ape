#version 450 core

// Source:
// https://www.geeks3d.com/20091027/shader-library-posterization-post-processing-effect-glsl/

in vec2 textureCoords;

out vec4 fragmentColor;

uniform sampler2D screenTexture;

uniform vec2 lensRadius = vec2(0.45, 0.38);

void main()
{
    vec4 color = texture2D(screenTexture, textureCoords);
  
    float dist = distance(textureCoords, vec2(0.5, 0.5));
  
    color.rgb *= smoothstep(lensRadius.x, lensRadius.y, dist);

    fragmentColor = color;
}
