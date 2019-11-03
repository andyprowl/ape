#pragma once

#include <glm/vec3.hpp>

namespace ape
{

class PlainCubeVertex
{

public:

    explicit PlainCubeVertex(glm::vec3 const & position)
        : position{position}
    {
    }

public:

    glm::vec3 position;

};

} // namespace ape
