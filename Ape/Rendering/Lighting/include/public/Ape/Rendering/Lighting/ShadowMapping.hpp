#pragma once

#include <Ape/Rendering/Lighting/DepthMapping.hpp>
#include <Ape/Rendering/Lighting/LightSystemView.hpp>

namespace ape
{

class ShadowMapping
{

public:

    ShadowMapping(LightSystem const & lightSystem, basix::Size<int> const & mapSize)
        : lightSystemView{lightSystem, mapSize}
        , depthMapping{lightSystem, mapSize}
    {
    }

public:

    LightSystemView lightSystemView;

    DepthMapping depthMapping;

};

} // namespace ape
