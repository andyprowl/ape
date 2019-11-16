#pragma once

#include <glm/vec3.hpp>

#include <array>

namespace ape
{

class AxisAlignedBox
{

public:

    AxisAlignedBox(glm::vec3 const & min, glm::vec3 const & max)
        : min{min}
        , max{max}
    {
    }

public:

    glm::vec3 min;

    glm::vec3 max;

};

} // namespace ape
