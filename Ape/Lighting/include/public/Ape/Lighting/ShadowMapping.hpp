#pragma once

#include <Ape/Lighting/DepthMapping.hpp>
#include <Ape/Lighting/LightSystemView.hpp>

namespace ape
{

class ShadowMapping
{

public:

    ShadowMapping(LightSystem const & lightSystem, Size<int> const & mapSize)
        : lightSystemView{lightSystem, mapSize}
        , depthMapping{lightSystem, mapSize}
    {
    }

public:

    LightSystemView lightSystemView;

    DepthMapping depthMapping;

};

} // namespace ape
