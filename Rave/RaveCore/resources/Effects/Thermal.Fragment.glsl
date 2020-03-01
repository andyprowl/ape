#version 460 core

// Source:
// https://www.geeks3d.com/20101123/shader-library-predators-thermal-vision-post-processing-filter-glsl/

in vec2 textureCoords;

out vec4 fragmentColor;

uniform sampler2D screenTexture;

void main() 
{   
    vec3 tc = vec3(1.0, 0.0, 0.0);

    vec3 textureColor = texture2D(screenTexture, textureCoords).rgb;

    vec3 colors[3];

    colors[0] = vec3(0.0, 0.0, 1.0);

    colors[1] = vec3(1.0, 1.0 ,0.0);

    colors[2] = vec3(1.0, 0.0, 0.0);

    float lum = (textureColor.r + textureColor.g + textureColor.b) / 3.0;

    int ix = (lum < 0.5) ? 0 : 1;

    tc = mix(colors[ix], colors[ix + 1], (lum - float(ix) * 0.5) / 0.5);
  
    fragmentColor = vec4(tc, 1.0);
}
