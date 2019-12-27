#pragma once

namespace ape
{

class Light;

class LightShadowCastingStatusUpdaterOverlay
{

public:

    auto updateLightShadowCastingStatus(Light & light)
        -> void;

};

} // namespace ape
