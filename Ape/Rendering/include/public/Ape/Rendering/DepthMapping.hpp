#pragma once

#include <Ape/Rendering/DepthMap.hpp>

#include <vector>

namespace ape
{

class Lighting;

class DepthMapping
{

public:

    DepthMapping(Lighting const & lighting, Size<int> const & mapSize);

    auto getPointMapping()
        -> std::vector<DepthMap> &;

    auto getPointMapping() const
        -> std::vector<DepthMap> const &;

    auto getSpotMapping() const
        -> std::vector<DepthMap> const &;

    auto getSpotMapping()
        -> std::vector<DepthMap> &;

    auto getDirectionalMapping()
        -> std::vector<DepthMap> &;

    auto getDirectionalMapping() const
        -> std::vector<DepthMap> const &;

private:

    Lighting const * lighting;

    Size<int> mapSize;

    std::vector<DepthMap> directionalMapping;

    std::vector<DepthMap> spotMapping;

    std::vector<DepthMap> pointMapping;

};

} // namespace ape
