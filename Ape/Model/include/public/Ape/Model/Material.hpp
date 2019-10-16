#pragma once

#include <Ape/GpuResource/Texture.hpp>

#include <glm/vec3.hpp>

namespace ape
{

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

} // namespace ape
