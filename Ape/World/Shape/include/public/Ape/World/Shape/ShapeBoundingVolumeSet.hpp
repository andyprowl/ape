#pragma once

#include <Ape/World/Shape/AxisAlignedBox.hpp>
#include <Ape/World/Shape/Sphere.hpp>

namespace ape
{

class ShapeBoundingVolumeSet
{

public:

    ShapeBoundingVolumeSet(AxisAlignedBox const & box, Sphere const & sphere)
        : box{box}
        , sphere{sphere}
    {
    }

public:

    AxisAlignedBox box;

    Sphere sphere;

};

} // namespace ape
