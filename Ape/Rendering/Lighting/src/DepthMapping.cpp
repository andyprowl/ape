#include <Ape/Rendering/Lighting/DepthMapping.hpp>

#include <Ape/World/Scene/LightSystem.hpp>

#include <Basix/Range/Transform.hpp>

namespace ape
{

DepthMapping::DepthMapping(LightSystem const & lightSystem, basix::Size2d<int> const & mapSize)
    : lightSystem{&lightSystem}
    , mapSize{mapSize}
    , point{mapSize, static_cast<int>(lightSystem.point.size()), "DepthMap.Point"}
    , spot{mapSize, static_cast<int>(lightSystem.spot.size()), "DepthMap.Spot"}
    , directional{mapSize, static_cast<int>(lightSystem.directional.size()), "DepthMap.Directional"}
{
}

auto DepthMapping::getMapSize() const
    -> basix::Size2d<int>
{
    return mapSize;
}

auto DepthMapping::getPointMapping()
    -> OmniDepthMap &
{
    return point;
}

auto DepthMapping::getPointMapping() const
    -> OmniDepthMap const &
{
    return point;
}

auto DepthMapping::getSpotMapping()
    -> MonoDepthMap &
{
    return spot;
}

auto DepthMapping::getSpotMapping() const
    -> MonoDepthMap const &
{
    return spot;
}

auto DepthMapping::getDirectionalMapping()
    -> MonoDepthMap &
{
    return directional;
}

auto DepthMapping::getDirectionalMapping() const
    -> MonoDepthMap const &
{
    return directional;
}

} // namespace ape
