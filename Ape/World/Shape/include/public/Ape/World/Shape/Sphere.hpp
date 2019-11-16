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

    auto getRadius() const
        -> float
    {
        return radius;
    }

    auto transform(glm::mat4 const & transformation)
    {
        center = glm::vec3{transformation * glm::vec4{center, 1.0f}};
    }

    auto translate(glm::vec3 const & offset)
    {
        center += offset;
    }

    auto scale(float const factor)
    {
        radius *= factor;
    }

private:

    glm::vec3 center;

    float radius;

};

} // namespace ape
