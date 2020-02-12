#pragma once

#include <Ape/Rendering/Lighting/DepthMapping.hpp>
#include <Ape/Rendering/Lighting/LightSystemView.hpp>

namespace ape
{

class ShadowMapping
{

public:

    ShadowMapping(LightSystemView & lightSystemView, basix::Size<int> const & mapSize)
        : lightSystemView{&lightSystemView}
        , depthMapping{lightSystemView.getLightSystem(), mapSize}
    {
    }

public:

    LightSystemView * lightSystemView;

    DepthMapping depthMapping;

};

} // namespace ape
