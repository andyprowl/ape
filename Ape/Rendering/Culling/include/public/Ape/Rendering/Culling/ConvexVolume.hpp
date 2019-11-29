#pragma once

#include <Ape/Rendering/Culling/Plane.hpp>

#include <Basix/Range/StaticVector.hpp>

namespace ape
{

class ConvexVolume
{

public:

    static constexpr auto maxNumOfPlanes = 18;

    using PlaneSet = basix::StaticVector<Plane, maxNumOfPlanes>;

public:

    auto addPlane(Plane const & plane)
        -> void
    {
        planes.push_back(plane);
    }

    auto getPlanes() const
        -> PlaneSet const &
    {
        return planes;
    }

private:

    PlaneSet planes;

};

} // namespace ape
