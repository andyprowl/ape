#pragma once

#include <Ape/Model/NormalDirection.hpp>
#include <Ape/Model/Shape.hpp>

namespace ape
{

class BoxBuilder
{

public:

    class Size : public glm::vec3
    {

    public:

        using glm::vec3::vec3;

        Size(glm::vec3 const & v)
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
        Size const & size = {1.0f, 1.0f, 1.0f},
        Position const & center = {0.0f, 0.0f, 0.0f}) const
        -> Shape;

};

} // namespace ape
