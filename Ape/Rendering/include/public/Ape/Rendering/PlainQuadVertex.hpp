#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace ape
{

class PlainQuadVertex
{

public:

    PlainQuadVertex(glm::vec2 const & position, glm::vec2 const & textureCoordinates)
        : position{position}
        , textureCoordinates{textureCoordinates}
    {
    }

public:

    glm::vec2 position;

    glm::vec2 textureCoordinates;

};

} // namespace ape
