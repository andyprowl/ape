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

    bool isCastingShadow;

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

    bool isCastingShadow;

};

struct DirectionalLight
{

    vec3 direction;

    LightColor color;

    bool isTurnedOn;

    bool isCastingShadow;

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

    samplerCubeShadow point[MAX_NUM_OF_POINT_LIGHTS];

    sampler2DShadow spot[MAX_NUM_OF_SPOT_LIGHTS];

    sampler2DShadow directional[MAX_NUM_OF_DIRECTIONAL_LIGHTS];

};

struct LightSpacePositioning
{

    vec4 spot[MAX_NUM_OF_SPOT_LIGHTS];

    vec4 directional[MAX_NUM_OF_DIRECTIONAL_LIGHTS];

};

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

    const float intensity = clamp((angleCosine - light.outerCutoffCosine) / epsilon, 0.0, 1.0);

    const vec3 ambientLight = computeAmbientLight(light.color);

    const vec3 diffuseLight = computeDiffuseLight(light.color, vertexToLight);

    const vec3 specularLight = computeSpecularLight(light.color, vertexToLight);

    const float sourceDistance = length(light.position - vertex.position);

    const float attenuation = computeAttenuationFactor(light.attenuation, sourceDistance);

    const vec3 color = (ambientLight + diffuseLight + specularLight);

    return (attenuation * intensity * min(vec3(1.0, 1.0, 1.0), color));
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

float calculateFogFactor(const float fogDensity)
{
    if (fogDensity == 0.0)
    {
        return 1.0;
    }

    const float heightLimit = 10.0;

    const vec3 cameraToVertex = camera.position - vertex.position;

    const float d = length(cameraToVertex);

    const float deltaH = abs(cameraToVertex.y);

    const float deltaHZero = (vertex.position.y > camera.position.y)
        ? max(0.0, vertex.position.y - heightLimit) - max(0.0, camera.position.y - heightLimit)
        : max(0.0, camera.position.y - heightLimit) - max(0.0, vertex.position.y - heightLimit);

    const float fd = d * ((deltaH - deltaHZero) / deltaH);

    const float fogFactor = exp(-2.0 * (fd * fogDensity) * (fd * fogDensity));
    
    return clamp(fogFactor, 0.0, 1.0);
}

float evaluateFogIntegral(float x, float a, float b, float c)
{
    const float disc = (4 * a * c) - (b * b);

    const float tx = (2 * a * x) + b;

    if (disc > 0.0)
    {
        const float droot = sqrt(disc);

        return (2.0 / droot) * atan(tx / droot);
    }
    else if (disc < 0.0)
    {
        const float droot = sqrt(-disc);

        return (1.0 / droot) * log((tx - droot) / (tx + droot));
    }
    else
    {
        return (-2.0 / tx);
    }
}

float calculateLightQuantity(const PointLight light)
{
    if (!light.isTurnedOn)
    {
        return 0.0;
    }

    const vec3 v = camera.position - vertex.position;

    const vec3 lp = vertex.position - light.position;

    const float a = dot(v, v);

    const float b = 2 * dot(v, lp);

    const float c = dot(lp, lp);

    return evaluateFogIntegral(1.0, a, b, c) - evaluateFogIntegral(0.0, a, b, c);
}

vec3 fog(vec3 color)
{
    const float fogDensity = 0.05;

    const vec3 fogColor = vec3(0.2, 0.1, 0.4);
    
    const float lightQuantity = calculateLightQuantity(lightSystem.point[0]);

    const float fogFactor = calculateFogFactor(fogDensity);

    vec3 result = mix(fogColor * lightQuantity, color, fogFactor);

    return result;
}

void main()
{
    if (renderNormals)
    {
        fragmentColor = vec4(getNormal() * 0.5 + 0.5, 1.0);
    }
    else
    {
        fragmentColor = vec4(fog(renderLighting()), 1.0);
    }
}
