#pragma once

#include <glm/mat4x4.hpp>
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

    auto getCenter() const
        -> glm::vec3 const &
    {
        return center;
    }

    auto setCenter(glm::vec3 const & newCenter)
        -> void
    {
        center = newCenter;
    }

    auto getRadius() const
        -> float
    {
        return radius;
    }

    auto getRadius(float const newRadius)
        -> void
    {
        radius = newRadius;
    }

    auto translate(glm::vec3 const & offset)
        -> void
    {
        center += offset;
    }

    auto scale(float const factor)
        -> void
    {
        radius *= factor;
    }

private:

    glm::vec3 center;

    float radius;

};

} // namespace ape
