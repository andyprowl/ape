#include <Ape/Rendering/Lighting/DepthMapping.hpp>

#include <Ape/World/Scene/LightSystem.hpp>

#include <Basix/Range/Transform.hpp>

namespace ape
{

namespace
{

auto makePointLightDepthMapping(
    std::vector<PointLight> const & lights,
    basix::Size2d<int> const & mapSize)
    -> std::vector<OmniDepthMap>
{
    return basix::transform(lights, [&mapSize] (PointLight const & light)
    {
        return OmniDepthMap{mapSize, std::string{light.getName()} + ".DepthMap"};
    });
}

} // unnamed namespace

DepthMapping::DepthMapping(LightSystem const & lightSystem, basix::Size2d<int> const & mapSize)
    : lightSystem{&lightSystem}
    , mapSize{mapSize}
    , pointMapping{makePointLightDepthMapping(lightSystem.point, mapSize)}
    , spotMapping{mapSize, static_cast<int>(lightSystem.spot.size()), "DepthMap.Spot"}
    , directionalMapping{
        mapSize, 
        static_cast<int>(lightSystem.directional.size()),
        "DepthMap.Directional"}
{
}

auto DepthMapping::getMapSize() const
    -> basix::Size2d<int>
{
    return mapSize;
}

auto DepthMapping::getPointMapping()
    -> std::vector<OmniDepthMap> &
{
    return pointMapping;
}

auto DepthMapping::getPointMapping() const
    -> std::vector<OmniDepthMap> const &
{
    return pointMapping;
}

auto DepthMapping::getSpotMapping()
    -> MonoDepthMap &
{
    return spotMapping;
}

auto DepthMapping::getSpotMapping() const
    -> MonoDepthMap const &
{
    return spotMapping;
}

auto DepthMapping::getDirectionalMapping()
    -> MonoDepthMap &
{
    return directionalMapping;
}

auto DepthMapping::getDirectionalMapping() const
    -> MonoDepthMap const &
{
    return directionalMapping;
}

} // namespace ape
