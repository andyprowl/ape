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

    sampler2D diffuse;

    sampler2D specular;

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

struct Lighting
{

    PointLight point[MAX_NUM_OF_POINT_LIGHTS];

    int pointArraySize;

    SpotLight spot[MAX_NUM_OF_SPOT_LIGHTS];

    int spotArraySize;

    DirectionalLight directional[MAX_NUM_OF_DIRECTIONAL_LIGHTS];

    int directionalArraySize;

};

struct LightingView
{

    //mat4 point[MAX_NUM_OF_POINT_LIGHTS];

    mat4 spot[MAX_NUM_OF_SPOT_LIGHTS];

    //mat4 directional[MAX_NUM_OF_DIRECTIONAL_LIGHTS];

};

struct DepthMapping
{

    //sampler2D point[MAX_NUM_OF_POINT_LIGHTS];

    sampler2D spot[MAX_NUM_OF_SPOT_LIGHTS];

    //sampler2D directional[MAX_NUM_OF_DIRECTIONAL_LIGHTS];

};

struct LightSpacePositioning
{

    //vec4 point[MAX_NUM_OF_POINT_LIGHTS];

    vec4 spot[MAX_NUM_OF_SPOT_LIGHTS];

    //vec4 directional[MAX_NUM_OF_DIRECTIONAL_LIGHTS];

};

in Vertex vertex;

in LightSpacePositioning lightSpacePositioning;

out vec4 fragmentColor;

uniform Camera camera;

uniform Material material;

uniform Lighting lighting;

uniform LightingView lightingView;

uniform DepthMapping depthMapping;

uniform bool useBlinnPhongModel;

float computeAttenuationFactor(Attenuation attenuation, float sourceDistance)
{    
    return 
        1.0 / 
        (attenuation.constant + 
         attenuation.linear * sourceDistance +
         attenuation.quadratic * (sourceDistance * sourceDistance));
}

float calculateShadowFactor(SpotLight light, vec4 lightSpacePosition, sampler2D depthMap)
{
    vec3 lightProjectionPosition = lightSpacePosition.xyz / lightSpacePosition.w;

    vec3 depthMapPosition = lightProjectionPosition * 0.5 + 0.5;

    float closestDepth = texture(depthMap, depthMapPosition.xy).r;

    float currentDepth = depthMapPosition.z;

    float bias = max(0.0002 * (1.0 - abs(dot(vertex.normal, light.direction))), 0.000005);
    
    return ((currentDepth - bias) > closestDepth) ? 0.0 : 1.0;
}

vec3 computeAmbientLight(LightColor color)
{
    vec3 diffuseColor = vec3(texture(material.diffuse, vertex.textureCoords));

    return color.ambient * material.ambient * diffuseColor;
}

vec3 computeDiffuseLight(LightColor color, vec3 lightDirection)
{
    float diffusion = max(dot(vertex.normal, lightDirection), 0.0);

    vec3 diffuseColor = vec3(texture(material.diffuse, vertex.textureCoords));

    return color.diffuse * (diffusion * diffuseColor);
}

float computeSpecularLightReflectivity(vec3 viewDirection, vec3 lightDirection)
{
    if (useBlinnPhongModel)
    {
        vec3 halfwayDirection = normalize(lightDirection + viewDirection);

        float shininessAdjuster = 4.0;

        float shininess = material.shininess * shininessAdjuster;

        return pow(max(dot(vertex.normal, halfwayDirection), 0.0), shininess);
    }
    else
    {
        // Uses classical Phong model

        vec3 reflectDirection = reflect(-lightDirection, vertex.normal);

        return pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    }
}

vec3 computeSpecularLight(LightColor color, vec3 lightDirection)
{
    vec3 viewDirection = normalize(camera.position - vertex.position);

    float reflectivity = computeSpecularLightReflectivity(viewDirection, lightDirection);

    vec3 specularColor = vec3(texture(material.specular, vertex.textureCoords));

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

    for (int i = 0; i < lighting.pointArraySize; ++i)
    {
        PointLight light = lighting.point[i];

        if (light.isTurnedOn)
        {
            color += computePointLight(light);
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

    for (int i = 0; i < lighting.spotArraySize; ++i)
    {
        SpotLight light = lighting.spot[i];

        if (!light.isTurnedOn)
        {
            continue;
        }

        vec4 lightSpacePosition = lightSpacePositioning.spot[i];

        float shadow = calculateShadowFactor(light, lightSpacePosition, depthMapping.spot[i]);

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

    for (int i = 0; i < lighting.directionalArraySize; ++i)
    {
        DirectionalLight light = lighting.directional[i];

        if (light.isTurnedOn)
        {
            color += computeDirectionalLight(light);
        }
    }

    return color;
}

void main()
{
    vec3 pointLighting = computePointLighting();

    vec3 spotLighting = computeSpotLighting();

    vec3 directionalLighting = computeDirectionalLighting();

    fragmentColor = vec4(pointLighting + directionalLighting + spotLighting, 1.0);
}
