#include <Ape/Rendering/Lighting/DepthMapping.hpp>

#include <Ape/World/Scene/LightSystem.hpp>

#include <Basix/Range/Transform.hpp>

namespace ape
{

namespace
{

auto makePointLightDepthMapping(
    std::vector<PointLight> const & lights,
    basix::Size<int> const & mapSize)
    -> std::vector<OmniDepthMap>
{
    return basix::transform(lights, [&mapSize] (PointLight const & light)
    {
        return OmniDepthMap{mapSize, std::string{light.getName()} + ".DepthMap"};
    });
}

template<typename LightContainer>
auto makeOrientedLightDepthMapping(
    LightContainer const & lights,
    basix::Size<int> const & mapSize)
    -> std::vector<MonoDepthMap>
{
    return basix::transform(lights, [&mapSize] (auto const & light)
    {
        return MonoDepthMap{mapSize, std::string{light.getName()} + ".DepthMap"};
    });
}

} // unnamed namespace

DepthMapping::DepthMapping(LightSystem const & lightSystem, basix::Size<int> const & mapSize)
    : lightSystem{&lightSystem}
    , mapSize{mapSize}
    , pointMapping{makePointLightDepthMapping(lightSystem.point, mapSize)}
    , spotMapping{makeOrientedLightDepthMapping(lightSystem.spot, mapSize)}
    , directionalMapping{makeOrientedLightDepthMapping(lightSystem.directional, mapSize)}
{
}

auto DepthMapping::getMapSize() const
    -> basix::Size<int>
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
    -> std::vector<MonoDepthMap> &
{
    return spotMapping;
}

auto DepthMapping::getSpotMapping() const
    -> std::vector<MonoDepthMap> const &
{
    return spotMapping;
}

auto DepthMapping::getDirectionalMapping()
    -> std::vector<MonoDepthMap> &
{
    return directionalMapping;
}

auto DepthMapping::getDirectionalMapping() const
    -> std::vector<MonoDepthMap> const &
{
    return directionalMapping;
}

} // namespace ape
