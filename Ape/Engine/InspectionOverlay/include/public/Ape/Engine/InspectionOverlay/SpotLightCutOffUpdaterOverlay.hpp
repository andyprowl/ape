#pragma once

#include <Ape/World/Scene/SpotLight.hpp>

namespace ape
{

class SpotLightCutoffUpdaterOverlay
{

public:

    auto updateLightCutoff(SpotLight & light)
        -> void;

private:

    auto updateOuterAngle(SpotLight::CutoffAngle & cutoff)
        -> bool;

    auto updateInnerAngle(SpotLight::CutoffAngle & cutoff)
        -> bool;

};

} // namespace ape
