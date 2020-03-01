#version 460 core

#include "BlinnPhong/BlinnPhong.Types.glsl"

in Vertex vertex;

in LightSpacePositioning lightSpacePositioning;

out vec4 fragmentColor;

uniform LightSystemUniformBlock
{

    LightSystem lightSystem;

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
    const vec3 lightPosition,
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

float computeAttenuationFactor(const Attenuation attenuation, const float sourceDistance)
{    
    return 
        1.0 / 
        (attenuation.constant +
         attenuation.quadratic * (sourceDistance * sourceDistance));
}

vec3 computeAmbientLight(const LightColor color)
{
    return (color.ambient * ambientColor);
}

vec3 computeDiffuseLight(const LightColor color, const vec3 lightDirection)
{
    if (!material.hasDiffuseMap)
    {
        return vec3(0.0, 0.0, 0.0);
    }

    const float diffusion = dot(mappedNormal, lightDirection);

    // If the normal is perpendicular to light direction (i.e. the non-bumped surface is parallel
    // to the light direction) we do not want the surface to be illuminated. Because of this, we
    // multiply the diffusion by the dot product of light and (non-bumped) surface normal.
    const float correctedDiffusion = diffusion * dot(vertex.normal, lightDirection);

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

vec3 computeSpecularLight(const LightColor color, const vec3 lightDirection)
{
    if (!material.hasSpecularMap)
    {
        return vec3(0.0, 0.0, 0.0);
    }

    const float reflectivity = computeSpecularLightReflectivity(lightDirection);

    // If the normal is perpendicular to light direction (i.e. the non-bumped surface is parallel
    // to the light direction) we do not want the surface to be illuminated. Because of this, we
    // multiply the reflectivity by the dot product of light and (non-bumped) surface normal.
    const float correctedReflectivity = reflectivity * dot(vertex.normal, lightDirection);

    return (color.specular * (correctedReflectivity * specularColor));
}

vec3 computePointLight(const PointLight light)
{
    const vec3 vertexToLight = normalize(light.position - vertex.position);

    const vec3 ambientLight = computeAmbientLight(light.color);

    const vec3 diffuseLight = computeDiffuseLight(light.color, vertexToLight);

    const vec3 specularLight = computeSpecularLight(light.color, vertexToLight);

    const float sourceDistance = length(light.position - vertex.position);

    const float attenuation = computeAttenuationFactor(light.attenuation, sourceDistance);

    return (attenuation * (ambientLight + diffuseLight + specularLight));
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
        
        const float shadow = calculatePointLightShadowFactor(
            light.isCastingShadow,
            light.position,
            i);

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

    const vec3 color = cutoff * (ambientLight + diffuseLight + specularLight);

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

    const vec3 ambientLight = computeAmbientLight(light.color);

    const vec3 diffuseLight = computeDiffuseLight(light.color, lightDirection);

    const vec3 specularLight = computeSpecularLight(light.color, lightDirection);

    return (ambientLight + diffuseLight + specularLight);
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
    const vec3 pointLighting = computePointLighting();

    const vec3 spotLighting = computeSpotLighting();

    const vec3 directionalLighting = computeDirectionalLighting();

    return (pointLighting + directionalLighting + spotLighting);
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
