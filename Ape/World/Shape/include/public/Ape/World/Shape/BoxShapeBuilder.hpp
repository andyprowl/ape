#pragma once

#include <Ape/World/Shape/NormalDirection.hpp>
#include <Ape/World/Shape/Shape.hpp>

namespace ape
{

class BoxShapeBuilder
{

public:

    class Size2d : public glm::vec3
    {

    public:

        using glm::vec3::vec3;

        Size2d(glm::vec3 const & v)
            : glm::vec3{v}
        {
        }

    };

    class Position : public glm::vec3
    {

    public:

        using glm::vec3::vec3;

        Position(glm::vec3 const & v)
            : glm::vec3{v}
        {
        }

    };

public:

    auto build(
        NormalDirection normalDirection,
        Size2d const & size = {1.0f, 1.0f, 1.0f},
        Position const & center = {0.0f, 0.0f, 0.0f},
        glm::vec2 const & textureTopRight = {1.0f, 1.0f}) const
        -> Shape;

};

} // namespace ape
