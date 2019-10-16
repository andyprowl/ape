#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace ape
{

class ShapeVertex
{

public:

    ShapeVertex(
        glm::vec3 const & position,
        glm::vec3 const & normal,
        glm::vec2 const & textureCoordinates)
        : position{position}
        , normal{normal}
        , textureCoordinates{textureCoordinates}
    {
    }

public:

    glm::vec3 position;

    glm::vec3 normal;

    glm::vec2 textureCoordinates;

};

} // namespace ape
