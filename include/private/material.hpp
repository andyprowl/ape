#pragma once

#include <glm/vec3.hpp>

class Material
{

public:

    Material(
        glm::vec3 const & ambient,
        glm::vec3 const & diffuse,
        glm::vec3 const & specular,
        float const shininess)
        : ambient{ambient}
        , diffuse{diffuse}
        , specular{specular}
        , shininess{shininess}
    {
    }

public:

    glm::vec3 ambient;

    glm::vec3 diffuse;

    glm::vec3 specular;

    float shininess;

};
