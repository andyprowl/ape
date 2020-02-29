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
        -> OmniDepthMap &;

    auto getPointMapping() const
        -> OmniDepthMap const &;

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

    OmniDepthMap point;

    MonoDepthMap spot;

    MonoDepthMap directional;

};

} // namespace ape
