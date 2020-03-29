#version 460 core

#include "BlinnPhong.Types.glsl"

in Vertex vertex;

in LightSpacePositioning lightSpacePositioning;

out vec4 fragmentColor;

uniform LightSystemUniformBlock
{

    LightSystem lightSystem;

};

uniform LightSystemViewUniformBlock
{

    LightSystemView lightSystemView;

};

uniform Camera camera;

uniform MaterialSetBlock
{

    int numOfMaterials;

    Material materials[1024];

};

uniform MaterialMaps materialMaps;

uniform int activeMaterialIndex;

uniform bool usePhongModel = false;

uniform bool useNormalMapping = true;

uniform bool renderNormals = false;

const Material material = materials[activeMaterialIndex];

const vec3 specularColor = vec3(texture(materialMaps.specularMap, vertex.textureCoords));

const vec3 diffuseColor = vec3(texture(materialMaps.diffuseMap, vertex.textureCoords));

const vec3 ambientColor = material.ambient * diffuseColor;

const vec3 viewDirection = normalize(camera.position - vertex.position);

// Extern, defined in separate shader.
float calculatePointLightShadowFactor(
    const bool isCastingShadow,
    const vec3 lightToVertex,
    const float distanceFromLight,
    const int layer);

// Extern, defined in separate shader.
float calculateSpotLightShadowFactor(
    const bool isCastingShadow,
    const vec3 lightDirection,
    vec4 depthMapPositionAndTestDepth,
    const int layer);

// Extern, defined in separate shader.
float calculateDirectionalLightShadowFactor(
    const bool isCastingShadow,
    const vec3 lightDirection,
    vec4 depthMapPositionAndTestDepth,
    const int layer);

// Extern, defined in separate shader.
vec3 applyFog(const vec3 color, const vec3 cameraToFragment);

vec3 getMappedNormalInTangentSpace()
{
    const vec3 sampledNormal = vec3(texture(materialMaps.normalMap, vertex.textureCoords));

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

vec3 mappedNormal = getNormal();

float computeAttenuationFactor(const Attenuation attenuation, const float squareDistance)
{    
    return (1.0 / (attenuation.constant + attenuation.quadratic * squareDistance));
}

vec3 computeAmbientLight(const LightColor color)
{
    return (color.ambient * ambientColor);
}

vec3 computeDiffuseLight(
    const LightColor color,
    const vec3 lightDirection,
    const float dotNormalLightDir)
{
    if (!material.hasDiffuseMap)
    {
        return vec3(0.0, 0.0, 0.0);
    }

    const float diffusion = dot(mappedNormal, lightDirection);

    // If the normal is perpendicular to light direction (i.e. the non-bumped surface is parallel
    // to the light direction) we do not want the surface to be illuminated. Because of this, we
    // multiply the diffusion by the dot product of light and (non-bumped) surface normal.
    const float correctedDiffusion = diffusion * dotNormalLightDir;

    const float cappedDiffusion = max(correctedDiffusion, 0.0);

    return (color.diffuse * (cappedDiffusion * diffuseColor));
}

float computeSpecularLightReflectivity(const vec3 lightDirection)
{
    if (usePhongModel)
    {
        // Uses classical Phong model

        const vec3 reflectDirection = reflect(-lightDirection, mappedNormal);

        return pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    }
    else
    {
        // Uses Blinn-Phong model

        const vec3 halfwayDirection = normalize(lightDirection + viewDirection);

        const float shininessAdjuster = 4.0;

        const float shininess = material.shininess * shininessAdjuster;

        const float factor = dot(mappedNormal, halfwayDirection);

        return pow(max(factor, 0.0), shininess);
    }
}

vec3 computeSpecularLight(
    const LightColor color,
    const vec3 lightDirection,
    const float dotNormalLightDir)
{
    if (!material.hasSpecularMap)
    {
        return vec3(0.0, 0.0, 0.0);
    }

    const float reflectivity = computeSpecularLightReflectivity(lightDirection);

    // If the normal is perpendicular to light direction (i.e. the non-bumped surface is parallel
    // to the light direction) we do not want the surface to be illuminated. Because of this, we
    // multiply the reflectivity by the dot product of light and (non-bumped) surface normal.
    const float correctedReflectivity = reflectivity * dotNormalLightDir;

    return (color.specular * (correctedReflectivity * specularColor));
}

vec3 computePointLight(
    const PointLight light,
    const vec3 vertexToLight,
    const float distanceFromLight,
    const float squareDistanceFromLight)
{
    const float attenuation = computeAttenuationFactor(light.attenuation, squareDistanceFromLight);

    // If attenuation is very low we can skip complex calculations and return a black color.
    // Note: conditionals are expensive, so it is not clear whether this brings a benefit.
    
    if (attenuation < 0.02)
    {
        return vec3(0.0, 0.0, 0.0);
    }

    const vec3 vertexToLightDir = vertexToLight / distanceFromLight;

    const float dotNormalLightDir = dot(vertex.normal, vertexToLightDir);

    const vec3 ambient = computeAmbientLight(light.color);

    const vec3 diffuse = computeDiffuseLight(light.color, vertexToLightDir, dotNormalLightDir);

    const vec3 specular = computeSpecularLight(light.color, vertexToLightDir, dotNormalLightDir);

    return (attenuation * (ambient + diffuse + specular));
}

vec3 computePointLighting()
{
    vec3 color = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < lightSystem.numOfPointLights; ++i)
    {
        const PointLight light = lightSystem.point[i];

        if (!light.isTurnedOn)
        {
            continue;
        }

        const vec3 vertexToLight = light.position - vertex.position;

        const float squareDistanceFromLight = dot(vertexToLight, vertexToLight);

        const float distanceFromLight = sqrt(squareDistanceFromLight);

        if (distanceFromLight > lightSystemView.point[i])
        {
            continue;
        }
        
        const float shadow = calculatePointLightShadowFactor(
            light.isCastingShadow,
            -vertexToLight,
            distanceFromLight,
            i);

        if (shadow > 0.0)
        {
            color += shadow * computePointLight(
                light,
                vertexToLight,
                distanceFromLight,
                squareDistanceFromLight);
        }
    }

    return color;
}

vec3 computeSpotLight(const SpotLight light)
{
    const vec3 vertexToLight = light.position - vertex.position;

    const float squareDistanceFromLight = dot(vertexToLight, vertexToLight);

    const float distanceFromLight = sqrt(squareDistanceFromLight);

    const vec3 vertexToLightDir = vertexToLight / distanceFromLight;

    // We are assuming light direction was normalized on the CPU side.
    const float angleCosine = dot(vertexToLightDir, -light.direction);
    
    const float epsilon = light.innerCutoffCosine - light.outerCutoffCosine;

    const float cutoff = clamp((angleCosine - light.outerCutoffCosine) / epsilon, 0.0, 1.0);

    const float attenuation = computeAttenuationFactor(light.attenuation, squareDistanceFromLight);

    // If cutoff and/or attenuation are very low we can skip the following calculations and just
    // return a black color.
    // Note: conditionals are expensive, so it is not clear whether this brings a benefit.
    
    if (cutoff * attenuation < 0.001)
    {
        return vec3(0.0, 0.0, 0.0);
    }

    const float dotNormalLightDir = dot(vertex.normal, vertexToLightDir);

    const vec3 ambient = computeAmbientLight(light.color);

    const vec3 diffuse = computeDiffuseLight(light.color, vertexToLightDir, dotNormalLightDir);

    const vec3 specular = computeSpecularLight(light.color, vertexToLightDir, dotNormalLightDir);

    const vec3 color = cutoff * (ambient + diffuse + specular);

    return (attenuation * min(vec3(1.0, 1.0, 1.0), color));
}

vec3 computeSpotLighting()
{
    vec3 color = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < lightSystem.numOfSpotLights; ++i)
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

        const float shadow = calculateSpotLightShadowFactor(
            light.isCastingShadow,
            light.direction,
            lightSpacePosition,
            i);

        if (shadow > 0.0)
        {
            color += shadow * computeSpotLight(light);
        }
    }

    return color;
}

vec3 computeDirectionalLight(const DirectionalLight light)
{
    // We are assuming light direction was normalized on the CPU side.
    const vec3 lightDirection = -light.direction;

    const float dotNormalLightDir = dot(vertex.normal, lightDirection);

    const vec3 ambient = computeAmbientLight(light.color);

    const vec3 diffuse = computeDiffuseLight(light.color, lightDirection, dotNormalLightDir);

    const vec3 specular = computeSpecularLight(light.color, lightDirection, dotNormalLightDir);

    return (ambient + diffuse + specular);
}

vec3 computeDirectionalLighting()
{
    vec3 color = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < lightSystem.numOfDirectionalLights; ++i)
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

        const float shadow = calculateDirectionalLightShadowFactor(
            light.isCastingShadow,
            light.direction,
            lightSpacePosition,
            i);

        if (shadow > 0.0)
        {
            color += shadow * computeDirectionalLight(light);
        }
    }

    return color;
}

vec3 renderLighting()
{
    const vec3 point = computePointLighting();

    const vec3 spot = computeSpotLighting();

    const vec3 directional = computeDirectionalLighting();

    return (point + spot + directional);
}

void main()
{
    if (renderNormals)
    {
        fragmentColor = vec4(mappedNormal * 0.5 + 0.5, 1.0);
    }
    else
    {
        vec3 color = renderLighting();

        vec3 cameraToFragment = vertex.position - camera.position;

        fragmentColor = vec4(applyFog(color, cameraToFragment), 1.0);
    }
}