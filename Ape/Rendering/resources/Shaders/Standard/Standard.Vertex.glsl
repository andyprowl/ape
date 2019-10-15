#version 450 core

// IMPORTANT:
// By convention, we call the uniform that contains the size of a uniform array UA as UAArraySize,
// so for example the size of the "point" array is held in a uniform named pointArraySize.
// The CPU side of the software relies on this naming convention.

#define MAX_NUM_OF_POINT_LIGHTS 8

#define MAX_NUM_OF_SPOT_LIGHTS 8

#define MAX_NUM_OF_DIRECTIONAL_LIGHTS 8

struct Vertex
{

    vec3 position;

    vec3 normal;

    vec2 textureCoords;

};

struct Transform
{

    mat4 model;

    mat4 camera;

    mat3 normal;

};

struct Camera
{

    vec3 position;

};

struct Material
{

    vec3 ambient;

    sampler2D diffuse;

    sampler2D specular;

    float shininess;

};

struct Attenuation
{

    float constant;

    float linear;

    float quadratic;

};

struct LightColor
{

    vec3 ambient;

    vec3 diffuse;

    vec3 specular;

};

struct PointLight
{

    vec3 position;

    LightColor color;

    Attenuation attenuation;

    bool isTurnedOn;

};

struct SpotLight
{

    vec3 position;

    vec3 direction;

    float innerCutoffCosine;

    float outerCutoffCosine;

    Attenuation attenuation;

    LightColor color;

    bool isTurnedOn;

};

struct DirectionalLight
{

    vec3 direction;

    LightColor color;

    bool isTurnedOn;

};

struct Lighting
{

    PointLight point[MAX_NUM_OF_POINT_LIGHTS];

    int pointArraySize;

    SpotLight spot[MAX_NUM_OF_SPOT_LIGHTS];

    int spotArraySize;

    DirectionalLight directional[MAX_NUM_OF_DIRECTIONAL_LIGHTS];

    int directionalArraySize;

};

struct LightingView
{

    //mat4 point[MAX_NUM_OF_POINT_LIGHTS];

    mat4 spot[MAX_NUM_OF_SPOT_LIGHTS];

    mat4 directional[MAX_NUM_OF_DIRECTIONAL_LIGHTS];

};

struct DepthMapping
{

    //sampler2D point[MAX_NUM_OF_POINT_LIGHTS];

    sampler2D spot[MAX_NUM_OF_SPOT_LIGHTS];

    sampler2D directional[MAX_NUM_OF_DIRECTIONAL_LIGHTS];

};

struct LightSpacePositioning
{

    //vec4 point[MAX_NUM_OF_POINT_LIGHTS];

    vec4 spot[MAX_NUM_OF_SPOT_LIGHTS];

    vec4 directional[MAX_NUM_OF_DIRECTIONAL_LIGHTS];

};

layout (location = 0) in vec3 positionAttribute;

layout (location = 1) in vec3 normalAttribute;

layout (location = 2) in vec2 textureCoordsAttribute;

out Vertex vertex;

out LightSpacePositioning lightSpacePositioning;

uniform Transform transform;

uniform Camera camera;

uniform Lighting lighting;

uniform LightingView lightingView;

void main()
{
    vec4 rawPosition = vec4(positionAttribute, 1.0);

    vec4 modelPosition = transform.model * rawPosition;

    gl_Position = transform.camera * rawPosition;

    vertex.position = vec3(modelPosition);

    vertex.normal = normalize(transform.normal * normalAttribute);

    vertex.textureCoords = vec2(1.0 - textureCoordsAttribute.x, textureCoordsAttribute.y);

    for (int i = 0; i < lighting.spotArraySize; ++i)
    {
        lightSpacePositioning.spot[i] = lightingView.spot[i] * modelPosition;
    }

    for (int i = 0; i < lighting.directionalArraySize; ++i)
    {
        lightSpacePositioning.directional[i] = lightingView.directional[i] * modelPosition;
    }
}
