#pragma once

#include <Basix/Meta/Counted.hpp>

#include <glm/vec3.hpp>

namespace glow
{

class Texture2d;

} // namespace glow

namespace ape
{

// Materials needs to be assigned a global, unique index that allows easily specifying the active
// material index for indexing the material uniform block in shaders.
class Material : public basix::Counted<Material>
{

public:

    Material(
        glm::vec3 const & ambient,
        glow::Texture2d const * diffuseMap,
        glow::Texture2d const * specularMap,
        glow::Texture2d const * normalMap,
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

    glow::Texture2d const * diffuseMap;

    glow::Texture2d const * specularMap;

    glow::Texture2d const * normalMap;

    float shininess;

};

} // namespace ape
