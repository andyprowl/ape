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
        -> MonoDepthMap const &;

    auto getSpotMapping()
        -> MonoDepthMap &;

    auto getDirectionalMapping()
        -> MonoDepthMap &;

    auto getDirectionalMapping() const
        -> MonoDepthMap const &;

private:

    LightSystem const * lightSystem;

    basix::Size2d<int> mapSize;

    std::vector<OmniDepthMap> pointMapping;

    MonoDepthMap spotMapping;

    MonoDepthMap directionalMapping;

};

} // namespace ape
