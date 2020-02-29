#define MAX_NUM_OF_POINT_LIGHTS 15

#define MAX_NUM_OF_SPOT_LIGHTS 8

#define MAX_NUM_OF_DIRECTIONAL_LIGHTS 4

struct Vertex
{

    vec3 position;

    vec3 normal;

    vec2 textureCoords;

    mat3 tangentToWorld;

};

struct Transform
{

    mat4 worldToClip;

    mat4 objectToWorld;

    mat3 normal;

};

struct Camera
{

    vec3 position;

};

struct Material
{

    vec3 ambient;

    float shininess;

    bool hasDiffuseMap;

    bool hasSpecularMap;

    bool hasNormalMap;

};

struct MaterialMaps
{

    sampler2D diffuseMap;

    sampler2D specularMap;

    sampler2D normalMap;

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

    int numOfPointLights;

    PointLight point[MAX_NUM_OF_POINT_LIGHTS];

    int numOfSpotLights;

    SpotLight spot[MAX_NUM_OF_SPOT_LIGHTS];

    int numOfDirectionalLights;

    DirectionalLight directional[MAX_NUM_OF_DIRECTIONAL_LIGHTS];

};

struct LightSystemView
{

    mat4 spot[MAX_NUM_OF_SPOT_LIGHTS];

    mat4 directional[MAX_NUM_OF_DIRECTIONAL_LIGHTS];

};

struct Fog
{
    
    float density;

    vec3 color;

};

struct DepthMapping
{

    samplerCubeArrayShadow point;

    sampler2DArrayShadow spot;

    sampler2DArrayShadow directional;

};

struct LightSpacePositioning
{

    vec4 spot[MAX_NUM_OF_SPOT_LIGHTS];

    vec4 directional[MAX_NUM_OF_DIRECTIONAL_LIGHTS];

};
