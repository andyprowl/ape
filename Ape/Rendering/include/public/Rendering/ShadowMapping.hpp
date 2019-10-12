#pragma once

#include <Rendering/DepthMapping.hpp>
#include <Rendering/LightingView.hpp>

namespace ape
{

class ShadowMapping
{

public:

    ShadowMapping(Lighting const & lighting, Size<int> const & mapSize)
        : lightingView{lighting, mapSize}
        , depthMapping{lighting, mapSize}
    {
    }

public:

    LightingView lightingView;

    DepthMapping depthMapping;

};

} // namespace ape
