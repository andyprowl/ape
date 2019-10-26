#include <Ape/Rendering/DepthMapping.hpp>

#include <Ape/Scene/Lighting.hpp>

#include <Foundational/Range/Transform.hpp>

namespace ape
{

namespace
{

template<typename LightContainer>
auto makeDepthMapping(LightContainer const & lights, Size<int> const & mapSize)
    -> std::vector<DepthMap>
{
    return transform(lights, [&mapSize] (auto const & /*light*/)
    {
        return DepthMap{mapSize};
    });
}

} // unnamed namespace

DepthMapping::DepthMapping(Lighting const & lighting, Size<int> const & mapSize)
    : lighting{&lighting}
    , mapSize{mapSize}
    , pointMapping{makeDepthMapping(lighting.point, mapSize)}
    , spotMapping{makeDepthMapping(lighting.spot, mapSize)}
    , directionalMapping{makeDepthMapping(lighting.directional, mapSize)}
{
}

auto DepthMapping::getPointMapping()
    -> std::vector<DepthMap> &
{
    return pointMapping;
}

auto DepthMapping::getPointMapping() const
    -> std::vector<DepthMap> const &
{
    return pointMapping;
}

auto DepthMapping::getSpotMapping()
    -> std::vector<DepthMap> &
{
    return spotMapping;
}

auto DepthMapping::getSpotMapping() const
    -> std::vector<DepthMap> const &
{
    return spotMapping;
}

auto DepthMapping::getDirectionalMapping()
    -> std::vector<DepthMap> &
{
    return directionalMapping;
}

auto DepthMapping::getDirectionalMapping() const
    -> std::vector<DepthMap> const &
{
    return directionalMapping;
}

} // namespace ape
