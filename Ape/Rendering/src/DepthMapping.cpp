#include <Ape/Rendering/DepthMapping.hpp>

#include <Ape/Scene/Lighting.hpp>

#include <Foundational/Range/Transform.hpp>

namespace ape
{

namespace
{

auto makePointLightDepthMapping(std::vector<PointLight> const & lights, Size<int> const & mapSize)
    -> std::vector<OmniDepthMap>
{
    return transform(lights, [&mapSize] (auto const & /*light*/)
    {
        return OmniDepthMap{mapSize};
    });
}

template<typename LightContainer>
auto makeOrientedLightDepthMapping(LightContainer const & lights, Size<int> const & mapSize)
    -> std::vector<MonoDepthMap>
{
    return transform(lights, [&mapSize] (auto const & /*light*/)
    {
        return MonoDepthMap{mapSize};
    });
}

} // unnamed namespace

DepthMapping::DepthMapping(Lighting const & lighting, Size<int> const & mapSize)
    : lighting{&lighting}
    , mapSize{mapSize}
    , pointMapping{makePointLightDepthMapping(lighting.point, mapSize)}
    , spotMapping{makeOrientedLightDepthMapping(lighting.spot, mapSize)}
    , directionalMapping{makeOrientedLightDepthMapping(lighting.directional, mapSize)}
{
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
