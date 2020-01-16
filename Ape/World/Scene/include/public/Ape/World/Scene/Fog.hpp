#pragma once

#include <glm/vec3.hpp>

namespace ape
{

class Fog
{

public:

    Fog()
        : Fog{0.0, {1.0, 1.0, 1.0}}
    {
    }

    Fog(float const density, glm::vec3 const & color)
        : density{density}
        , color{color}
    {
    }

public:

    float density;

    glm::vec3 color;

};

} // namespace ape
