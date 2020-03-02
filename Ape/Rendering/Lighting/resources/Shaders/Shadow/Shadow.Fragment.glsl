#version 460 core

#include "BlinnPhong/BlinnPhong.Types.glsl"

in Vertex vertex;

in LightSpacePositioning lightSpacePositioning;

uniform DepthMapping depthMapping;

uniform bool usePercentageCloserFiltering = false;

float calculateOmnidirectionalShadowBias(const vec3 lightToVertex, const float distanceFromLight)
{
    return 0.00005 * distanceFromLight * (1.0 - dot(vertex.normal, lightToVertex));
}

float calculateMonodirectionalShadowBias(const vec3 lightDirection)
{
    // Notice: not using bumped normal!
    
    const float sine = length(cross(vertex.normal, lightDirection));
    
    const float cosine = abs(dot(vertex.normal, lightDirection));
    
    const float tangent = sine / cosine;
    
    return clamp(0.005 * tangent, 0.0, 0.001);

    // ALTERNATIVE TECHNIQUE below: seems to be OK for spot lights, but not for directional lights.
    // We probably shouldn't use bumped normals for shadow mapping as they generate more artifacts.
    
    // return max(0.0002 * (1.0 - abs(dot(vertex.normal, lightDirection))), 0.000005);
}

float sampleShadowWithPercentageCloserFiltering(
    const sampler2DArrayShadow depthMap,
    const vec4 depthMapPositionAndTestDepth)
{
    float shadow = 0.0;

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            shadow += textureOffset(depthMap, depthMapPositionAndTestDepth, ivec2(x, y)).r;
        }
    }

    shadow /= 9.0;

    return shadow;
}

float calculateMonodirectionalShadowFactor(
    const bool isCastingShadow,
    const vec3 lightDirection,
    vec4 depthMapPositionAndTestDepth,
    const sampler2DArrayShadow depthMap,
    const int layer)
{
    if (!isCastingShadow)
    {
        return 1.0;
    }

    const float bias = calculateMonodirectionalShadowBias(lightDirection);

    depthMapPositionAndTestDepth /= depthMapPositionAndTestDepth.w;

    depthMapPositionAndTestDepth.w = depthMapPositionAndTestDepth.z - bias;

    depthMapPositionAndTestDepth.z = float(layer);

    if (usePercentageCloserFiltering)
    {
        return sampleShadowWithPercentageCloserFiltering(depthMap, depthMapPositionAndTestDepth);
    }
    else
    {
        return texture(depthMap, depthMapPositionAndTestDepth).r;
    }
}

float calculateSpotLightShadowFactor(
    const bool isCastingShadow,
    const vec3 lightDirection,
    vec4 depthMapPositionAndTestDepth,
    const int layer)
{
    return calculateMonodirectionalShadowFactor(
        isCastingShadow,
        lightDirection,
        depthMapPositionAndTestDepth,
        depthMapping.spot,
        layer);
}

float calculateDirectionalLightShadowFactor(
    const bool isCastingShadow,
    const vec3 lightDirection,
    vec4 depthMapPositionAndTestDepth,
    const int layer)
{
    return calculateMonodirectionalShadowFactor(
        isCastingShadow,
        lightDirection,
        depthMapPositionAndTestDepth,
        depthMapping.directional,
        layer);
}

float calculatePointLightShadowFactor(
    const bool isCastingShadow,
    const vec3 lightToVertex,
    const float distanceFromLight,
    const int layer)
{
    if (!isCastingShadow)
    {
        return 1.0;
    }

    // TODO: This can be different for each light! This is a hack.
    // The far plane should either be passed a part of the light uniform or the algorithm should be
    // changed to not make use of the far plane.
    const float farPlaneDistance = 100.0;

    const float currentDepthNormalized = distanceFromLight / farPlaneDistance;

    const float bias = calculateOmnidirectionalShadowBias(lightToVertex, distanceFromLight);

    const vec4 coords = vec4(lightToVertex, float(layer));

    return texture(depthMapping.point, coords, currentDepthNormalized - bias).r;
}
