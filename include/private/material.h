#pragma once

#include <glm/vec3.hpp>

class Material
{

public:

    Material(
        glm::vec3 const & ambientColor,
        glm::vec3 const & diffuseColor,
        glm::vec3 const & specularColor,
        float const shininess)
        : ambientColor{ambientColor}
        , diffuseColor{diffuseColor}
        , specularColor{specularColor}
        , shininess{shininess}
    {
    }

public:

    glm::vec3 ambientColor;

    glm::vec3 diffuseColor;

    glm::vec3 specularColor;

    float shininess;

};
