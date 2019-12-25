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
    
    vec3 tangent;

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
    vec3 sampledNormal = vec3(texture(material.normalMap, vertex.textureCoords));

    return normalize(sampledNormal * 2.0 - vec3(1.0, 1.0, 1.0));
}

mat3 computeTangentToWorldTransform()
{
    vec3 normal = normalize(vertex.normal);

    vec3 tangent = normalize(vertex.tangent - dot(vertex.tangent, normal) * normal);

    vec3 bitangent = normalize(cross(normal, tangent));

    return mat3(tangent, bitangent, normal);
}

vec3 getSampledBumpedNormal()
{
    vec3 normalInTangentSpace = getMappedNormalInTangentSpace();

    // TODO: Figure out how to properly handle normal maps with alpha channels and "null" texels
    // such as (0, 0, 0, 0). Doing this should not be necessary and the problem should likely be
    // solved at the origin (i.e. model contains normal maps which are not normal maps).
    if (normalInTangentSpace.z < 0.0)
    {
        return vertex.normal;
    }

    mat3 tangentToWorld = computeTangentToWorldTransform();

    vec3 normalInWorldSpace = tangentToWorld * normalInTangentSpace;

    return normalize(normalInWorldSpace);
}

vec3 getNormal()
{
    if (material.hasNormalMap && useNormalMapping)
    {
        vec3 bumpedNormal = getSampledBumpedNormal();

        return bumpedNormal;
    }
    else
    {
        return vertex.normal;
    }
}

float computeAttenuationFactor(Attenuation attenuation, float sourceDistance)
{    
    return 
        1.0 / 
        (attenuation.constant + 
         attenuation.linear * sourceDistance +
         attenuation.quadratic * (sourceDistance * sourceDistance));
}

float sampleShadowWithPercentageCloserFiltering(
    vec3 depthMapPositionAndTestDepth,
    sampler2DShadow depthMap)
{
    float testDepth = depthMapPositionAndTestDepth.z;

    vec2 texelSize = 1.0 / textureSize(depthMap, 0);

    float shadow = 0.0;

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            vec2 xyOffsets = vec2(x, y) * texelSize;

            vec2 xySamplingCoords = depthMapPositionAndTestDepth.xy + xyOffsets;

            shadow += texture(depthMap, vec3(xySamplingCoords, testDepth)).r;
        }
    }

    shadow /= 9.0;

    return shadow;
}

float calculateMonodirectionalShadowFactor(
    vec3 lightDirection,
    vec4 lightSpacePosition,
    sampler2DShadow depthMap)
{
    vec3 lightProjectionPosition = lightSpacePosition.xyz / lightSpacePosition.w;

    vec3 depthMapPositionAndTestDepth = lightProjectionPosition * 0.5 + 0.5;

    float bias = max(0.0002 * (1.0 - abs(dot(getNormal(), lightDirection))), 0.000005);

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

float calculateOmnidirectionalShadowFactor(vec3 lightPosition, samplerCubeShadow depthMap)
{
    // TODO: This can be different for each light! This is a hack.
    // The far plane should either be passed a part of the light uniform or the algorithm should be
    // changed to not make use of the far plane.
    const float farPlaneDistance = 100.0;

    vec3 lightToVertex = vertex.position - lightPosition;

    float currentDepthNormalized = length(lightToVertex) / farPlaneDistance;

    float bias = 0.002;

    vec4 coords = vec4(lightToVertex, currentDepthNormalized - bias);

    return texture(depthMap, coords).r;
}

vec3 computeAmbientLight(LightColor color)
{
    vec3 diffuseColor = vec3(texture(material.diffuseMap, vertex.textureCoords));

    return color.ambient * material.ambient * diffuseColor;
}

vec3 computeDiffuseLight(LightColor color, vec3 lightDirection)
{
    if (!material.hasDiffuseMap)
    {
        return vec3(0.0, 0.0, 0.0);
    }

    float diffusion = max(dot(getNormal(), lightDirection), 0.0);

    vec3 diffuseColor = vec3(texture(material.diffuseMap, vertex.textureCoords));

    return color.diffuse * (diffusion * diffuseColor);
}

float computeSpecularLightReflectivity(vec3 viewDirection, vec3 lightDirection)
{
    if (usePhongModel)
    {
        // Uses classical Phong model

        vec3 reflectDirection = reflect(-lightDirection, getNormal());

        return pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    }
    else
    {
        // Uses Blinn-Phong model

        vec3 halfwayDirection = normalize(lightDirection + viewDirection);

        float shininessAdjuster = 4.0;

        float shininess = material.shininess * shininessAdjuster;

        return pow(max(dot(getNormal(), halfwayDirection), 0.0), shininess);
    }
}

vec3 computeSpecularLight(LightColor color, vec3 lightDirection)
{
    if (!material.hasSpecularMap)
    {
        return vec3(0.0, 0.0, 0.0);
    }

    vec3 viewDirection = normalize(camera.position - vertex.position);

    float reflectivity = computeSpecularLightReflectivity(viewDirection, lightDirection);

    vec3 specularColor = vec3(texture(material.specularMap, vertex.textureCoords));

    return color.specular * (reflectivity * specularColor);
}

vec3 computePointLight(PointLight light)
{
    vec3 lightDirection = normalize(light.position - vertex.position);

    vec3 ambientLight = computeAmbientLight(light.color);

    vec3 diffuseLight = computeDiffuseLight(light.color, lightDirection);

    vec3 specularLight = computeSpecularLight(light.color, lightDirection);

    float sourceDistance = length(light.position - vertex.position);

    float attenuation = computeAttenuationFactor(light.attenuation, sourceDistance);

    return attenuation * (ambientLight + diffuseLight + specularLight);
}

vec3 computePointLighting()
{
    vec3 color = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < lightSystem.pointArraySize; ++i)
    {
        PointLight light = lightSystem.point[i];

        if (!light.isTurnedOn)
        {
            continue;
        }

        float shadow = calculateOmnidirectionalShadowFactor(
            light.position,
            depthMapping.point[i]);

        if (shadow > 0.0)
        {
            vec3 contribution = computePointLight(light);

            color += shadow * contribution;
        }
    }

    return color;
}

vec3 computeSpotLight(SpotLight light)
{
    vec3 lightDirection = normalize(light.position - vertex.position);

    float angleCosine = dot(lightDirection, normalize(-light.direction));
    
    float epsilon = light.innerCutoffCosine - light.outerCutoffCosine;

    float intensity = clamp((angleCosine - light.outerCutoffCosine) / epsilon, 0.0, 1.0);

    vec3 ambientLight = computeAmbientLight(light.color);

    vec3 diffuseLight = computeDiffuseLight(light.color, lightDirection);

    vec3 specularLight = computeSpecularLight(light.color, lightDirection);

    float sourceDistance = length(light.position - vertex.position);

    float attenuation = computeAttenuationFactor(light.attenuation, sourceDistance);

    return attenuation * intensity * (ambientLight + diffuseLight + specularLight);
}

vec3 computeSpotLighting()
{
    vec3 color = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < lightSystem.spotArraySize; ++i)
    {
        SpotLight light = lightSystem.spot[i];

        if (!light.isTurnedOn)
        {
            continue;
        }

        vec4 lightSpacePosition = lightSpacePositioning.spot[i];

        float shadow = calculateMonodirectionalShadowFactor(
            light.direction,
            lightSpacePosition,
            depthMapping.spot[i]);

        if (shadow > 0.0)
        {
            vec3 contribution = computeSpotLight(light);

            color += shadow * contribution;
        }
    }

    return color;
}

vec3 computeDirectionalLight(DirectionalLight light)
{
    vec3 lightDirection = normalize(-light.direction);

    vec3 ambientLight = computeAmbientLight(light.color);

    vec3 diffuseLight = computeDiffuseLight(light.color, lightDirection);

    vec3 specularLight = computeSpecularLight(light.color, lightDirection);

    return (ambientLight + diffuseLight + specularLight);
}

vec3 computeDirectionalLighting()
{
    vec3 color = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < lightSystem.directionalArraySize; ++i)
    {
        DirectionalLight light = lightSystem.directional[i];

        if (!light.isTurnedOn)
        {
            continue;
        }

        vec4 lightSpacePosition = lightSpacePositioning.directional[i];

        float shadow = calculateMonodirectionalShadowFactor(
            light.direction,
            lightSpacePosition,
            depthMapping.directional[i]);

        if (shadow > 0.0)
        {
            vec3 contribution = computeDirectionalLight(light);

            color += shadow * contribution;
        }
    }

    return color;
}

void renderLighting()
{
    vec3 pointLighting = computePointLighting();

    vec3 spotLighting = computeSpotLighting();

    vec3 directionalLighting = computeDirectionalLighting();

    fragmentColor = vec4(pointLighting + directionalLighting + spotLighting, 1.0);
}

void main()
{
    if (renderNormals)
    {
        fragmentColor = vec4(getNormal() * 0.5 + 0.5, 1.0);
    }
    else
    {
        renderLighting();
    }
}
