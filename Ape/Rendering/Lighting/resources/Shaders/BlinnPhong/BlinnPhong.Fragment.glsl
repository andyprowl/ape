#version 450 core

#include "BlinnPhong/BlinnPhong.Types.glsl"

in Vertex vertex;

in LightSpacePositioning lightSpacePositioning;

out vec4 fragmentColor;

uniform Camera camera;

uniform Material material;

uniform LightSystem lightSystem;

uniform LightSystemView lightSystemView;

uniform DepthMapping depthMapping;

uniform bool usePhongModel = false;

uniform bool usePercentageCloserFiltering = false;

uniform bool useNormalMapping = true;

uniform float fogDensity = 0.02;

uniform bool renderNormals = false;

vec3 getMappedNormalInTangentSpace()
{
    const vec3 sampledNormal = vec3(texture(material.normalMap, vertex.textureCoords));

    return normalize(sampledNormal * 2.0 - vec3(1.0, 1.0, 1.0));
}

vec3 getSampledBumpedNormal()
{
    const vec3 normalInTangentSpace = getMappedNormalInTangentSpace();

    const vec3 normalInWorldSpace = vertex.tangentToWorld * normalInTangentSpace;

    return normalInWorldSpace;
}

vec3 getNormal()
{
    if (material.hasNormalMap && useNormalMapping)
    {
        return getSampledBumpedNormal();
    }
    else
    {
        return vertex.normal;
    }
}

float computeAttenuationFactor(const Attenuation attenuation, const float sourceDistance)
{    
    return 
        1.0 / 
        (attenuation.constant + 
         attenuation.linear * sourceDistance +
         attenuation.quadratic * (sourceDistance * sourceDistance));
}

float sampleShadowWithPercentageCloserFiltering(
    const vec3 depthMapPositionAndTestDepth,
    const sampler2DShadow depthMap)
{
    const float testDepth = depthMapPositionAndTestDepth.z;

    const vec2 texelSize = 1.0 / textureSize(depthMap, 0);

    float shadow = 0.0;

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            const vec2 xyOffsets = vec2(x, y) * texelSize;

            const vec2 xySamplingCoords = depthMapPositionAndTestDepth.xy + xyOffsets;

            shadow += texture(depthMap, vec3(xySamplingCoords, testDepth)).r;
        }
    }

    shadow /= 9.0;

    return shadow;
}

float calculateMonodirectionalShadowBias(const vec3 lightDirection)
{
    // TODO: EXPERIMENTAL
    // Notice: not using bumped normal!
    
    const float sine = length(cross(vertex.normal, lightDirection));
    
    const float cosine = abs(dot(vertex.normal, lightDirection));
    
    const float tangent = sine / cosine;
    
    return clamp(0.005 * tangent, 0.0, 0.001);

    // ALTERNATIVE TECHNIQUE below: seems to be OK for spot lights, but not for directional lights.
    // We probably shouldn't use bumped normals for shadow mapping as they generate more artifacts.
    
    // return max(0.0002 * (1.0 - abs(dot(vertex.normal, lightDirection))), 0.000005);
}

float calculateMonodirectionalShadowFactor(
    const bool isCastingShadow,
    const vec3 lightDirection,
    const vec3 lightProjectionPosition,
    const sampler2DShadow depthMap)
{
    if (!isCastingShadow)
    {
        return 1.0;
    }

    vec3 depthMapPositionAndTestDepth = lightProjectionPosition * 0.5 + 0.5;

    const float bias = calculateMonodirectionalShadowBias(lightDirection);

    depthMapPositionAndTestDepth.z -= bias;

    if (usePercentageCloserFiltering)
    {
        return sampleShadowWithPercentageCloserFiltering(depthMapPositionAndTestDepth, depthMap);
    }
    else
    {
        return texture(depthMap, depthMapPositionAndTestDepth).r;
    }
}

float calculateOmnidirectionalShadowBias(/*const vec3 lightToVertex*/)
{
    // TODO: Figure out if we can use the same calcualtion as for monodirectional lights
    // return calculateMonodirectionalShadowBias(normalize(lightToVertex));
    
    return 0.0005;
}

float calculateOmnidirectionalShadowFactor(
    const bool isCastingShadow,
    const vec3 lightPosition,
    const samplerCubeShadow depthMap)
{
    if (!isCastingShadow)
    {
        return 1.0;
    }

    // TODO: This can be different for each light! This is a hack.
    // The far plane should either be passed a part of the light uniform or the algorithm should be
    // changed to not make use of the far plane.
    const float farPlaneDistance = 100.0;

    const vec3 lightToVertex = vertex.position - lightPosition;

    const float lightToVertexDistance = length(lightToVertex);

    const float currentDepthNormalized = lightToVertexDistance / farPlaneDistance;

    const float bias = calculateOmnidirectionalShadowBias(
        /*lightToVertex / lightToVertexDistance*/);

    const vec4 coords = vec4(lightToVertex, currentDepthNormalized - bias);

    return texture(depthMap, coords).r;
}

vec3 computeAmbientLight(const LightColor color)
{
    const vec3 diffuseColor = vec3(texture(material.diffuseMap, vertex.textureCoords));

    return (color.ambient * material.ambient * diffuseColor);
}

vec3 computeDiffuseLight(const LightColor color, const vec3 lightDirection)
{
    if (!material.hasDiffuseMap)
    {
        return vec3(0.0, 0.0, 0.0);
    }

    const float diffusion = dot(getNormal(), lightDirection);

    // If the normal is perpendicular to light direction (i.e. the non-bumped surface is parallel
    // to the light direction) we do not want the surface to be illuminated. Because of this, we
    // multiply the diffusion by the dot product of light and (non-bumped) surface normal.
    const float correctedDiffusion = diffusion * dot(vertex.normal, lightDirection);

    const float cappedDiffusion = max(correctedDiffusion, 0.0);

    const vec3 diffuseColor = vec3(texture(material.diffuseMap, vertex.textureCoords));

    return (color.diffuse * (cappedDiffusion * diffuseColor));
}

float computeSpecularLightReflectivity(const vec3 viewDirection, const vec3 lightDirection)
{
    if (usePhongModel)
    {
        // Uses classical Phong model

        const vec3 reflectDirection = reflect(-lightDirection, getNormal());

        return pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    }
    else
    {
        // Uses Blinn-Phong model

        const vec3 halfwayDirection = normalize(lightDirection + viewDirection);

        const float shininessAdjuster = 4.0;

        const float shininess = material.shininess * shininessAdjuster;

        const float factor = dot(getNormal(), halfwayDirection);

        return pow(max(factor, 0.0), shininess);
    }
}

vec3 computeSpecularLight(const LightColor color, const vec3 lightDirection)
{
    if (!material.hasSpecularMap)
    {
        return vec3(0.0, 0.0, 0.0);
    }

    const vec3 viewDirection = normalize(camera.position - vertex.position);

    const float reflectivity = computeSpecularLightReflectivity(viewDirection, lightDirection);

    // If the normal is perpendicular to light direction (i.e. the non-bumped surface is parallel
    // to the light direction) we do not want the surface to be illuminated. Because of this, we
    // multiply the reflectivity by the dot product of light and (non-bumped) surface normal.
    const float correctedReflectivity = reflectivity * dot(vertex.normal, lightDirection);

    const vec3 specularColor = vec3(texture(material.specularMap, vertex.textureCoords));

    return (color.specular * (correctedReflectivity * specularColor));
}

vec3 computePointLight(const PointLight light)
{
    const vec3 lightDirection = normalize(light.position - vertex.position);

    const vec3 ambientLight = computeAmbientLight(light.color);

    const vec3 diffuseLight = computeDiffuseLight(light.color, lightDirection);

    const vec3 specularLight = computeSpecularLight(light.color, lightDirection);

    const float sourceDistance = length(light.position - vertex.position);

    const float attenuation = computeAttenuationFactor(light.attenuation, sourceDistance);

    return (attenuation * (ambientLight + diffuseLight + specularLight));
}

vec3 computePointLighting()
{
    vec3 color = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < lightSystem.pointArraySize; ++i)
    {
        const PointLight light = lightSystem.point[i];

        if (!light.isTurnedOn)
        {
            continue;
        }
        
        const float shadow = calculateOmnidirectionalShadowFactor(
            light.isCastingShadow,
            light.position,
            depthMapping.point[i]);

        if (shadow > 0.0)
        {
            color += shadow * computePointLight(light);
        }
    }

    return color;
}

vec3 computeSpotLight(const SpotLight light)
{
    const vec3 vertexToLight = normalize(light.position - vertex.position);

    // We are assuming light direction was normalized on the CPU side.
    const float angleCosine = dot(vertexToLight, -light.direction);
    
    const float epsilon = light.innerCutoffCosine - light.outerCutoffCosine;

    const float cutoff = clamp((angleCosine - light.outerCutoffCosine) / epsilon, 0.0, 1.0);

    const vec3 ambientLight = computeAmbientLight(light.color);

    const vec3 diffuseLight = computeDiffuseLight(light.color, vertexToLight);

    const vec3 specularLight = computeSpecularLight(light.color, vertexToLight);

    const float sourceDistance = length(light.position - vertex.position);

    const float attenuation = computeAttenuationFactor(light.attenuation, sourceDistance);

    const vec3 color = (ambientLight + diffuseLight + specularLight);

    return (attenuation * cutoff * min(vec3(1.0, 1.0, 1.0), color));
}

vec3 computeSpotLighting()
{
    vec3 color = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < lightSystem.spotArraySize; ++i)
    {
        const SpotLight light = lightSystem.spot[i];

        if (!light.isTurnedOn)
        {
            continue;
        }

        const vec4 lightSpacePosition = lightSpacePositioning.spot[i];

        // If the fragment is outside of the light's view space, we don't need to perform any
        // calculation for it. This optimization seems to have a relevant impact on performance.
        if ((abs(lightSpacePosition.x) > lightSpacePosition.w) ||
            (abs(lightSpacePosition.y) > lightSpacePosition.w) ||
            (abs(lightSpacePosition.z) > lightSpacePosition.w))
        {
            continue;
        }

        const vec3 lightProjectionPosition = lightSpacePosition.xyz / lightSpacePosition.w;

        const float shadow = calculateMonodirectionalShadowFactor(
            light.isCastingShadow,
            light.direction,
            lightProjectionPosition,
            depthMapping.spot[i]);

        if (shadow > 0.0)
        {
            color += computeSpotLight(light);
        }
    }

    return color;
}

vec3 computeDirectionalLight(const DirectionalLight light)
{
    // We are assuming light direction was normalized on the CPU side.
    const vec3 lightDirection = -light.direction;

    const vec3 ambientLight = computeAmbientLight(light.color);

    const vec3 diffuseLight = computeDiffuseLight(light.color, lightDirection);

    const vec3 specularLight = computeSpecularLight(light.color, lightDirection);

    return (ambientLight + diffuseLight + specularLight);
}

vec3 computeDirectionalLighting()
{
    vec3 color = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < lightSystem.directionalArraySize; ++i)
    {
        const DirectionalLight light = lightSystem.directional[i];

        if (!light.isTurnedOn)
        {
            continue;
        }

        const vec4 lightSpacePosition = lightSpacePositioning.directional[i];

        // If the fragment is outside of the light's view space, we don't need to perform any
        // calculation for it. This optimization seems to have a relevant impact on performance.
        if ((abs(lightSpacePosition.x) > lightSpacePosition.w) ||
            (abs(lightSpacePosition.y) > lightSpacePosition.w) ||
            (abs(lightSpacePosition.z) > lightSpacePosition.w))
        {
            continue;
        }

        const vec3 lightProjectionPosition = lightSpacePosition.xyz / lightSpacePosition.w;

        const float shadow = calculateMonodirectionalShadowFactor(
            light.isCastingShadow,
            light.direction,
            lightProjectionPosition,
            depthMapping.directional[i]);

        if (shadow > 0.0)
        {
            color += shadow * computeDirectionalLight(light);
        }
    }

    return color;
}

vec3 renderLighting()
{
    const vec3 pointLighting = computePointLighting();

    const vec3 spotLighting = computeSpotLighting();

    const vec3 directionalLighting = computeDirectionalLighting();

    return (pointLighting + directionalLighting + spotLighting);
}

// Extern, define in separate shader.
vec3 fog(vec3 color, vec3 cameraToFragment);

void main()
{
    if (renderNormals)
    {
        fragmentColor = vec4(getNormal() * 0.5 + 0.5, 1.0);
    }
    else
    {
        vec3 color = renderLighting();

        if (fogDensity > 0.0)
        {
            vec3 cameraToFragment = vertex.position - camera.position;

            fragmentColor = vec4(fog(color, cameraToFragment), 1.0);
        }
        else
        {
            fragmentColor = vec4(color, 1.0);
        }
    }
}
