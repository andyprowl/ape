#pragma once

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>

namespace ape
{

class Plane
{

public:

    Plane(glm::vec3 const & normal, float const offset)
        : normal{glm::normalize(normal)}
        , offset{offset}
    {
    }

public:

    glm::vec3 normal;

    float offset;

};

auto computeSignedDistance(glm::vec3 const & point, Plane const & plane)
    -> float;

} // namespace ape
