#pragma once

namespace ape
{

template<typename LightType>
class LightPositionUpdaterOverlay
{

public:

    auto updateLightPosition(LightType & light)
        -> void;

};

} // namespace ape
