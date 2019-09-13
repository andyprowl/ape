#pragma once

#include <glm/vec3.hpp>

class Material
{

public:

    Material(
        glm::vec3 const & ambient,
        int const diffuseMapId,
        int const specularMapId,
        float const shininess)
        : ambient{ambient}
        , diffuseMapId{diffuseMapId}
        , specularMapId{specularMapId}
        , shininess{shininess}
    {
    }

public:

    glm::vec3 ambient;

    int diffuseMapId;

    int specularMapId;

    float shininess;

};
