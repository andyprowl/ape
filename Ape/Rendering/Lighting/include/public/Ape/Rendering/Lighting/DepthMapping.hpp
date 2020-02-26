#pragma once

#include <Ape/Rendering/Lighting/OmniDepthMap.hpp>
#include <Ape/Rendering/Lighting/MonoDepthMap.hpp>

#include <vector>

namespace ape
{

class LightSystem;

class DepthMapping
{

public:

    DepthMapping(LightSystem const & lightSystem, basix::Size2d<int> const & mapSize);

    auto getMapSize() const
        -> basix::Size2d<int>;

    auto getPointMapping()
        -> std::vector<OmniDepthMap> &;

    auto getPointMapping() const
        -> std::vector<OmniDepthMap> const &;

    auto getSpotMapping() const
        -> std::vector<MonoDepthMap> const &;

    auto getSpotMapping()
        -> std::vector<MonoDepthMap> &;

    auto getDirectionalMapping()
        -> std::vector<MonoDepthMap> &;

    auto getDirectionalMapping() const
        -> std::vector<MonoDepthMap> const &;

private:

    LightSystem const * lightSystem;

    basix::Size2d<int> mapSize;

    std::vector<OmniDepthMap> pointMapping;

    std::vector<MonoDepthMap> spotMapping;

    std::vector<MonoDepthMap> directionalMapping;

};

} // namespace ape
