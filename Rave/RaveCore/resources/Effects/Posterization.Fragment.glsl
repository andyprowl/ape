#version 460 core

// Source:
// https://www.geeks3d.com/20091027/shader-library-posterization-post-processing-effect-glsl/

in vec2 textureCoords;

out vec4 fragmentColor;

uniform sampler2D screenTexture;

uniform float gamma = 0.6;

uniform float numColors = 8.0;

void main() 
{ 
    vec3 c = texture2D(screenTexture, textureCoords.xy).rgb;

    c = pow(c, vec3(gamma, gamma, gamma));
  
    c = c * numColors;
  
    c = floor(c);
  
    c = c / numColors;
  
    c = pow(c, vec3(1.0/gamma));
  
    fragmentColor = vec4(c, 1.0);
}