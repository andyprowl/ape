#pragma once

#include <glm/vec3.hpp>

namespace ape
{

class Sphere
{

public:

    Sphere(glm::vec3 const& center, float const radius)
        : center{center}
        , radius{radius}
    {
    }

public:

    glm::vec3 center;

    float radius;

};

} // namespace ape
