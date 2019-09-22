#pragma once

#include <Ape/Texture.hpp>

#include <glm/vec3.hpp>

class Material
{

public:

    Material(
        glm::vec3 const & ambient,
        Texture const * diffuseMap,
        Texture const * specularMap,
        float const shininess)
        : ambient{ambient}
        , diffuseMap{diffuseMap}
        , specularMap{specularMap}
        , shininess{shininess}
    {
    }

public:

    glm::vec3 ambient;

    Texture const * diffuseMap;

    Texture const * specularMap;

    float shininess;

};
