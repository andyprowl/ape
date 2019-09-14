#pragma once

#include "Texture.hpp"

#include <glm/vec3.hpp>

class Material
{

public:

    Material(
        glm::vec3 const & ambient,
        Texture diffuseMap,
        Texture specularMap,
        float const shininess)
        : ambient{ambient}
        , diffuseMap{std::move(diffuseMap)}
        , specularMap{std::move(specularMap)}
        , shininess{shininess}
    {
    }

public:

    glm::vec3 ambient;

    Texture diffuseMap;

    Texture specularMap;

    float shininess;

};
