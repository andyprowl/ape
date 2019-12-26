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

    mat3 tangentToWorld;

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

    bool hasDiffuseMap;

    sampler2D diffuseMap;

    bool hasSpecularMap;

    sampler2D specularMap;

    bool hasNormalMap;

    sampler2D normalMap;

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

struct LightSystem
{

    PointLight point[MAX_NUM_OF_POINT_LIGHTS];

    int pointArraySize;

    SpotLight spot[MAX_NUM_OF_SPOT_LIGHTS];

    int spotArraySize;

    DirectionalLight directional[MAX_NUM_OF_DIRECTIONAL_LIGHTS];

    int directionalArraySize;

};

struct LightSystemView
{

    mat4 spot[MAX_NUM_OF_SPOT_LIGHTS];

    mat4 directional[MAX_NUM_OF_DIRECTIONAL_LIGHTS];

};

struct DepthMapping
{

    samplerCube point[MAX_NUM_OF_POINT_LIGHTS];

    sampler2D spot[MAX_NUM_OF_SPOT_LIGHTS];

    sampler2D directional[MAX_NUM_OF_DIRECTIONAL_LIGHTS];

};

struct LightSpacePositioning
{

    vec4 spot[MAX_NUM_OF_SPOT_LIGHTS];

    vec4 directional[MAX_NUM_OF_DIRECTIONAL_LIGHTS];

};

layout (location = 0) in vec3 positionAttribute;

layout (location = 1) in vec3 normalAttribute;

layout (location = 2) in vec3 tangentAttribute;

layout (location = 3) in vec2 textureCoordsAttribute;

out Vertex vertex;

out LightSpacePositioning lightSpacePositioning;

uniform Transform transform;

uniform Camera camera;

uniform LightSystem lightSystem;

uniform LightSystemView lightSystemView;

// This implements the so-called Gram-Schmidt process
vec3 orthogonalize(vec3 v, vec3 reference)
{
    return normalize(v - dot(v, reference) * reference);
}

vec3 computeTangentInWorldSpace(vec3 tangentInModelSpace, vec3 normalInWorldSpace)
{
    vec3 tangentInWorldSpace = normalize(transform.normal * tangentInModelSpace);

    return orthogonalize(tangentInWorldSpace, normalInWorldSpace);
}

mat3 computeTangentToWorld(vec3 tangentInModelSpace, vec3 normalInWorldSpace)
{
    vec3 tangent = computeTangentInWorldSpace(tangentInModelSpace, normalInWorldSpace);

    vec3 bitangent = cross(normalInWorldSpace, tangent);

    return mat3(tangent, bitangent, normalInWorldSpace);
}

void main()
{
    vec4 rawPosition = vec4(positionAttribute, 1.0);

    vec4 worldPosition = transform.model * rawPosition;

    gl_Position = transform.camera * rawPosition;

    vertex.position = vec3(worldPosition);

    vertex.normal = normalize(transform.normal * normalAttribute);

    vertex.tangentToWorld = computeTangentToWorld(tangentAttribute, vertex.normal);

    vertex.textureCoords = vec2(1.0 - textureCoordsAttribute.x, textureCoordsAttribute.y);

    // For point lights we do not need to compute the vertex position in light space.
    // This is due to how omnidirectional shadow maps are implemented.
    // We only need to do this for monodirectional shadow maps (spot and directional lights).

    for (int i = 0; i < lightSystem.spotArraySize; ++i)
    {
        lightSpacePositioning.spot[i] = lightSystemView.spot[i] * worldPosition;
    }

    for (int i = 0; i < lightSystem.directionalArraySize; ++i)
    {
        lightSpacePositioning.directional[i] = lightSystemView.directional[i] * worldPosition;
    }
}
