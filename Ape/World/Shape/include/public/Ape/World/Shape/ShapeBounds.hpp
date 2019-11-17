#pragma once

#include <Ape/World/Shape/Box.hpp>
#include <Ape/World/Shape/Sphere.hpp>

namespace ape
{

class ShapeBounds
{

public:

    ShapeBounds(Box const & box, Sphere const & sphere)
        : box{box}
        , sphere{sphere}
    {
    }

    auto getBox() const
        -> Box const &
    {
        return box;
    }

    auto getSphere() const
        -> Sphere const &
    {
        return sphere;
    }

    auto transform(glm::mat4 const & transformation)
        -> void;

    auto translate(glm::vec3 const & offset)
        -> void;

    auto scale(float const factor)
        -> void;

private:

    Box box;

    Sphere sphere;

};

} // namespace ape
