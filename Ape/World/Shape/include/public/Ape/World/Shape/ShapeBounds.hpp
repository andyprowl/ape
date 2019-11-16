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
        -> void
    {
        box.transform(transformation);

        sphere.transform(transformation);
    }

    auto translate(glm::vec3 const & offset)
    {
        box.translate(offset);

        sphere.translate(offset);
    }

    auto scale(float const factor)
        -> void
    {
        box.scale(factor);

        sphere.scale(factor);
    }

private:

    Box box;

    Sphere sphere;

};

} // namespace ape
