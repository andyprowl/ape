#version 450 core

#include "BlinnPhong/BlinnPhong.Types.glsl"

uniform Camera camera;

uniform LightSystem lightSystem;

uniform Fog fog;

// Returns a value in the range ]0.0, 1.0].
// A value of 0.0 indicates that only fog color should be considered to render the fragment.
// A value of 1.0 indicates that fog should not be considered at all to render the fragment.
float calculateFogFactor(const float distance)
{
    return exp(-(distance * fog.density));
}

/**
 * Atan implementation taken from:
 * https://seblagarde.wordpress.com/2014/12/01/inverse-trigonometric-functions-gpu-optimization-for-amd-gcn-architecture/
 * Polynomial degree = 2
 * Tune for positive input [0, infinity] and provide output [0, PI/2]
**/
float fastAtanPositive(const float x)
{
    #define PI 3.1415926535897932384626433832795
    #define HALF_PI (PI * 0.5)
    #define C1 1.01991
    #define C2 -0.218891

    const float t0 = (x < 1.0f) ? x : 1.0f / x;

    const float t1 = (C2 * t0 + C1) * t0; // p(x)
    
    // Undo range reduction
    return (x < 1.0f) ? t1 : HALF_PI - t1;
}

// input [-infinity, infinity] and output [-PI/2, PI/2]
float fastAtan(const float x)
{
    const float t0 = fastAtanPositive(abs(x));

    // Undo range reduction, branchless form of:
    //     return (x < 0.0) ? -t0 : +t0;
    return sign(x) * t0;
}

float evaluateLightQuantityIntegral(
    const float x,
    const float a,
    const float b,
    const float drootInverse)
{
    const float tx = (2.0 * a * x) + b;

    /**
     * Discriminant is always > 0.0 (needs proof), so we can avoid the conditionals that we would
     * theoretically need. If it weren't, we would have to handle all three cases as in the snippet
     * below:
     *
     *     const float disc = (4 * a * c) - (b * b);
     *     if (disc > 0.0)
     *     {
     *         const float droot = sqrt(disc);
     *         return (2.0 / droot) * atan(tx / droot);
     *     }
     *     else if (disc < 0.0)
     *     {
     *         const float droot = sqrt(-disc);
     *         return (1.0 / droot) * log((tx - droot) / (tx + droot));
     *     }
     *     else
     *     {
     *         return (-2.0 / tx);
     *     }
    **/

    // TODO: atan() is expensive, use a lookup table or some approximation
    return (2.0 * drootInverse) * fastAtan(tx * drootInverse);
}

float evaluateLightQuantityIntegral(
    const vec3 v,
    const float from,
    const float to,
    const float a,
    const float b,
    const float c)
{
    if (from >= to)
    {
        return 0.0;
    }

    // Discriminant is always > 0.0 (needs proof), so the square root is well-defined...
    const float disc = (4.0 * a * c) - (b * b);
    const float droot = sqrt(disc);
    const float drootInverse = 1.0 / droot;

    return
        evaluateLightQuantityIntegral(to, a, b, drootInverse) -
        evaluateLightQuantityIntegral(from, a, b, drootInverse);
}

float calculateNormalizedPointLightQuantity(
    const vec3 v,
    const vec3 lc,
    const float vLengthSquared)
{
    const float a = vLengthSquared;
    const float b = 2.0 * dot(v, lc);
    const float u = 0.1; // To avoid singularity when v and lp are parallel and opposing
    const float c = dot(lc, lc) + u;

    return evaluateLightQuantityIntegral(v, 0.0, 1.0, a, b, c);
}

float calculateNormalizedSpotLightQuantity(
    const vec3 v,
    const vec3 lc,
    const vec3 d,
    const float vLengthSquared,
    const float cutoffCosine)
{
    const float vDotD = dot(v, d);
    const float vDotLC = dot(v, lc);
    const float lcDotD = dot(lc, d);
    const float lcDotLC = dot(lc, lc);

    const float a = vLengthSquared;
    const float b = 2.0 * vDotLC;
    const float u = 0.1; // To avoid singularity when v and lp are parallel
    const float c = lcDotLC + u;
    
    const float k = 1.0 / (cutoffCosine * cutoffCosine);
    const float ta = vLengthSquared - k * vDotD * vDotD;
    const float tb = 2.0 * (vDotLC - k * vDotD * lcDotD);
    const float tc = lcDotLC - k * lcDotD * lcDotD;

    /**
     * The discriminant is always going to be >= 0.0, because the maths model intersection of a
     * parameteric line we have a *two-sided* cone, so there are always going to be either two or
     * infinite intersection points.
    **/
    const float tdisc = tb * tb - 4.0 * ta * tc;
    const float tdroot = sign(ta) * sqrt(tdisc);
    const float t1 = (-tb - tdroot) / (2.0 * ta);
    const float t2 = (-tb + tdroot) / (2.0 * ta);
    const bool p1 = (dot(t1 * v + lc, d) > 0.0);
    const bool p2 = (dot(t2 * v + lc, d) > 0.0);

    if (p1 == p2)
    {
        return float(p1) * evaluateLightQuantityIntegral(v, max(0.0, t1), min(1.0, t2), a, b, c);
    }
    else
    {
        return
            float(p1) * evaluateLightQuantityIntegral(v, 0.0, min(t1, 1.0), a, b, c) +
            float(p2) * evaluateLightQuantityIntegral(v, max(0.0, t2), 1.0, a, b, c);
    }
}

vec3 calculateNormalizedPointLitFog(const vec3 cameraToFragment, const float squaredDistance)
{
    vec3 color = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < lightSystem.pointArraySize; ++i)
    {
        const PointLight light = lightSystem.point[i];

        if (!light.isTurnedOn)
        {
            continue;
        }

        const vec3 lc = camera.position - light.position;

        const float quantity = calculateNormalizedPointLightQuantity(
            cameraToFragment,
            lc,
            squaredDistance);

        color += light.color.diffuse * (quantity / light.attenuation.quadratic);
    }

    return color;
}

vec3 calculateNormalizedSpotLitFog(const vec3 cameraToFragment, const float squaredDistance)
{
    vec3 color = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < lightSystem.spotArraySize; ++i)
    {
        const SpotLight light = lightSystem.spot[i];

        if (!light.isTurnedOn)
        {
            continue;
        }

        const vec3 lc = camera.position - light.position;

        const float quantity = calculateNormalizedSpotLightQuantity(
            cameraToFragment,
            lc,
            light.direction,
            squaredDistance,
            light.outerCutoffCosine);

        color += light.color.diffuse * (quantity / light.attenuation.quadratic);
    }

    return color;
}

vec3 calculateNormalizedDirectionalLitFog(const vec3 cameraToFragment, const float squaredDistance)
{
    vec3 color = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < lightSystem.directionalArraySize; ++i)
    {
        const DirectionalLight light = lightSystem.directional[i];

        if (!light.isTurnedOn)
        {
            continue;
        }

        color += light.color.diffuse;
    }

    return color;
}

// Taken from https://stackoverflow.com/a/4275343/1932150
float rand(vec2 v)
{
    return fract(sin(dot(v.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec3 calculateFogColor(const vec3 color, const vec3 cameraToFragment)
{
    const float squaredDistance = dot(cameraToFragment, cameraToFragment);

    const vec3 normalizedLitFogColor = 
        calculateNormalizedPointLitFog(cameraToFragment, squaredDistance) +
        calculateNormalizedSpotLitFog(cameraToFragment, squaredDistance) +
        calculateNormalizedDirectionalLitFog(cameraToFragment, squaredDistance);

    const float distance = sqrt(squaredDistance);

    // This helps reducing regular artefacts against the skybox.
    const float noise = 0.001 * rand(cameraToFragment.xy);

    const vec3 litFogColor = 0.01 * normalizedLitFogColor * fog.color * distance + noise;

    const float fogFactor = calculateFogFactor(distance);

    return mix(litFogColor, color, fogFactor);
}

vec3 applyFog(const vec3 color, const vec3 cameraToFragment)
{
    if (fog.density == 0.0)
    {
        return color;
    }

    return calculateFogColor(color, cameraToFragment);
}
