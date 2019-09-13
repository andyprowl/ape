#pragma once

#include <glm/vec3.hpp>

class Light
{

public:

    Light(
        glm::vec3 const & position,
        glm::vec3 const & ambient,
        glm::vec3 const & diffuse,
        glm::vec3 const & specular)
        : position{position}
        , ambient{ambient}
        , diffuse{diffuse}
        , specular{specular}
    {
    }

public:

    glm::vec3 position;

    glm::vec3 ambient;

    glm::vec3 diffuse;

    glm::vec3 specular;

};
