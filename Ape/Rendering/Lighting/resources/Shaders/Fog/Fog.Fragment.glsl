#version 450 core

#include "BlinnPhong/BlinnPhong.Types.glsl"

uniform Camera camera;

uniform LightSystem lightSystem;

uniform float fogDensity = 0.02;

float calculateFogFactor(const float fogDensity, const float lenSquare)
{
/*
    const float heightLimit = 10.0;

    const vec3 cameraToVertex = camera.position - vertex.position;

    const float d = length(cameraToVertex);

    const float deltaH = abs(cameraToVertex.y);

    const float deltaHZero = (vertex.position.y > camera.position.y)
        ? max(0.0, vertex.position.y - heightLimit) - max(0.0, camera.position.y - heightLimit)
        : max(0.0, camera.position.y - heightLimit) - max(0.0, vertex.position.y - heightLimit);

    const float fd = d * ((deltaH - deltaHZero) / deltaH);
*/
    return exp(-(lenSquare * fogDensity));
}

float evaluateLightQuantityIntegral(float x, float a, float b, float drootInverse)
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
    return (2.0 * drootInverse) * atan(tx * drootInverse);
}

float evaluateLightQuantityIntegral(float from, float to, float a, float b, float c)
{
    if (from >= to)
    {
        return 0.0;
    }

    const float disc = (4.0 * a * c) - (b * b);

    // Discriminant is always > 0.0 (needs proof), so the square root is well-defined...
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

    return evaluateLightQuantityIntegral(0.0, 1.0, a, b, c);
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
    const float u = 0.1; // To avoid singularity when v and lp are parallel and opposing
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
        return float(p1) * evaluateLightQuantityIntegral(max(0.0, t1), min(1.0, t2), a, b, c);
    }
    else
    {
        return
            float(p1) * evaluateLightQuantityIntegral(0.0, min(t1, 1.0), a, b, c) +
            float(p2) * evaluateLightQuantityIntegral(max(0.0, t2), 1.0, a, b, c);
    }
}

vec3 fog(vec3 color, vec3 fragmentPosition)
{
    const vec3 v = fragmentPosition - camera.position;

    const float a = dot(v, v);

    vec3 fogColor = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < lightSystem.pointArraySize; ++i)
    {
        const PointLight light = lightSystem.point[i];

        if (!light.isTurnedOn)
        {
            continue;
        }

        const vec3 lc = camera.position - light.position;

        fogColor += light.color.diffuse * calculateNormalizedPointLightQuantity(v, lc, a);
    }

    for (int i = 0; i < lightSystem.spotArraySize; ++i)
    {
        const SpotLight light = lightSystem.spot[i];

        if (!light.isTurnedOn)
        {
            continue;
        }

        const vec3 lc = camera.position - light.position;

        fogColor += light.color.diffuse * calculateNormalizedSpotLightQuantity(
            v,
            lc,
            light.direction,
            a,
            light.outerCutoffCosine);
    }

    for (int i = 0; i < lightSystem.directionalArraySize; ++i)
    {
        const DirectionalLight light = lightSystem.directional[i];

        if (!light.isTurnedOn)
        {
            continue;
        }

        // TODO: Replace this hardcoded value with a meaningful calculation.
        fogColor += light.color.diffuse * 0.1;
    }

    const float fogFactor = calculateFogFactor(fogDensity, a);

    const vec3 fogBaseColor = vec3(0.2, 0.2, 0.2);

    const float distance = sqrt(a);

    fogColor *= fogBaseColor * distance;

    return mix(fogColor, color, fogFactor);
}
