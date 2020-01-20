#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace ape
{

class ShapeVertex
{

public:

    static constexpr auto const numOfComponents = 4;

public:

    ShapeVertex(
        glm::vec3 const & position,
        glm::vec3 const & normal,
        glm::vec3 const & tangent,
        glm::vec2 const & textureCoordinates)
        : position{position}
        , normal{normal}
        , tangent{tangent}
        , textureCoordinates{textureCoordinates}
    {
    }

public:

    glm::vec3 position;

    glm::vec3 normal;

    glm::vec3 tangent;

    glm::vec2 textureCoordinates;

};

} // namespace ape
