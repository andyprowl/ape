#pragma once

namespace ape
{

template<typename LightType>
class LightDirectionUpdaterOverlay
{

public:

    auto updateLightDirection(LightType & light)
        -> void;

};

} // namespace ape
