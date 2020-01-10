#version 450 core

#include "BlinnPhong/BlinnPhong.Types.glsl"

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
    const vec3 tangentInWorldSpace = normalize(transform.normal * tangentInModelSpace);

    return orthogonalize(tangentInWorldSpace, normalInWorldSpace);
}

mat3 computeTangentToWorld(vec3 tangentInModelSpace, vec3 normalInWorldSpace)
{
    const vec3 tangent = computeTangentInWorldSpace(tangentInModelSpace, normalInWorldSpace);

    const vec3 bitangent = cross(normalInWorldSpace, tangent);

    return mat3(tangent, bitangent, normalInWorldSpace);
}

void main()
{
    const vec4 rawPosition = vec4(positionAttribute, 1.0);

    const vec4 worldPosition = transform.model * rawPosition;

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
