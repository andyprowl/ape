#pragma once

namespace ape
{

class Light;

class LightOnOffStatusUpdaterOverlay
{

public:

    auto updateLightOnOffStatus(Light & light)
        -> void;

};

} // namespace ape
