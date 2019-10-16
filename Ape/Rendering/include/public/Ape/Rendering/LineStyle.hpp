#pragma once

#include <glm/vec3.hpp>

namespace ape
{

class LineStyle
{

public:

    LineStyle(float const width, glm::vec3 const & color)
        : width{width}
        , color{color}
    {
    }

public:

    float width;

    glm::vec3 color;

};

} // namespace ape
