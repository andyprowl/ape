#pragma once

#include <glm/vec3.hpp>

namespace glow
{

class Texture;

} // namespace glow

namespace ape
{
class Material
{

public:

    Material(
        glm::vec3 const & ambient,
        glow::Texture const * diffuseMap,
        glow::Texture const * specularMap,
        glow::Texture const * normalMap,
        float const shininess)
        : ambient{ambient}
        , diffuseMap{diffuseMap}
        , specularMap{specularMap}
        , normalMap{normalMap}
        , shininess{shininess}
    {
    }

public:

    glm::vec3 ambient;

    glow::Texture const * diffuseMap;

    glow::Texture const * specularMap;

    glow::Texture const * normalMap;

    float shininess;

};

} // namespace ape
