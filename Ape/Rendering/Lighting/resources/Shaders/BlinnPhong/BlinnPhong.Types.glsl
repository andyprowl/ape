#define MAX_NUM_OF_POINT_LIGHTS 10

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

struct Fog
{
    
    float density;

    vec3 color;

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
