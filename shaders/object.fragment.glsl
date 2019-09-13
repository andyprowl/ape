// #include is a custom extension
#include "vertex.glsi"

struct Material
{

    vec3 ambient;

    sampler2D diffuse;

    sampler2D specular;

    float shininess;

};

struct Light
{

    vec3 ambient;

    vec3 diffuse;

    vec3 specular;

    vec3 position;

};

in Vertex vertex;

uniform Material material;

uniform Light light;

uniform vec3 viewPosition;

vec3 computeAmbientLight()
{
    vec3 diffuseColor = vec3(texture(material.diffuse, vertex.textureCoords));

    return light.ambient * material.ambient * diffuseColor;
}

vec3 computeDiffuseLight(vec3 lightDirection)
{
    float diffusion = max(dot(vertex.normal, lightDirection), 0.0);

    vec3 diffuseColor = vec3(texture(material.diffuse, vertex.textureCoords));

    return light.diffuse * (diffusion * diffuseColor);
}

vec3 computeSpecularLight(vec3 lightDirection)
{
    vec3 viewDirection = normalize(viewPosition - vertex.position);

    vec3 reflectDirection = reflect(-lightDirection, vertex.normal);

    float reflection = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);

    vec3 specularColor = vec3(texture(material.specular, vertex.textureCoords));

    return light.specular * (reflection * specularColor);
}

void main()
{
    vec3 ambientLight = computeAmbientLight();

    vec3 lightDirection = normalize(light.position - vertex.position);

    vec3 diffuseLight = computeDiffuseLight(lightDirection);

    vec3 specularLight = computeSpecularLight(lightDirection);

    gl_FragColor = vec4(ambientLight + diffuseLight + specularLight, 1.0);
}
