#pragma once

#include <Ape/Rendering/Culling/Plane.hpp>

#include <array>

namespace ape
{

class Frustum
{

public:

    enum class Face
    {
        right,
        left,
        top,
        bottom,
        front,
        back
    };

public:

    Frustum(
        Plane const & right,
        Plane const & left,
        Plane const & top,
        Plane const & bottom,
        Plane const & front,
        Plane const & back)
        : planes{{right, left, top, bottom, front, back}}
    {
    }

    auto getPlane(Face const face) const
        -> Plane const &
    {
        return planes[static_cast<std::size_t>(face)];
    }

    auto getPlanes() const
        -> std::array<Plane, 6u> const &
    {
        return planes;
    }

private:

    std::array<Plane, 6u> planes;

};

} // namespace ape
