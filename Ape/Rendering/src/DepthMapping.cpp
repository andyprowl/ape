#include <Rendering/DepthMapping.hpp>

#include <Scene/Lighting.hpp>

#include <Range/Transform.hpp>

namespace ape
{

namespace
{

auto makeSpotDepthMapping(Lighting const & lighting, Size<int> const & mapSize)
    -> std::vector<DepthMap>
{
    return transform(lighting.spot, [&mapSize] (auto const & ...)
    {
        return DepthMap{mapSize};
    });
}

} // unnamed namespace

DepthMapping::DepthMapping(Lighting const & lighting, Size<int> const & mapSize)
    : lighting{&lighting}
    , mapSize{mapSize}
    , spotMapping{makeSpotDepthMapping(lighting, mapSize)}
{
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

} // namespace ape
