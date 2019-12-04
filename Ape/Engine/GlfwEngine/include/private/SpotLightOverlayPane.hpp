#pragma once

#include "LightAttenuationUpdaterOverlay.hpp"
#include "LightColorUpdaterOverlay.hpp"
#include "LightDirectionUpdaterOverlay.hpp"
#include "LightOnOffStatusUpdaterOverlay.hpp"
#include "LightPositionUpdaterOverlay.hpp"
#include "LightSystemOverlayPane.hpp"
#include "SpotLightCutoffUpdaterOverlay.hpp"

#include <Ape/World/Scene/SpotLight.hpp>

namespace ape
{

class SpotLightOverlayPane : public LightSystemOverlayPane<SpotLight>
{

public:

    using LightSystemOverlayPane::LightSystemOverlayPane;

private:

    // virtual (from LightSystemOverlyPane<SpotLight>)
    auto updateSelectedLightProperties()
        -> void override;

    auto updateViewProperties(SpotLight & light)
        -> void;

private:

    LightOnOffStatusUpdaterOverlay onOffStatusUpdater;

    LightColorUpdaterOverlay colorUpdater;

    LightAttenuationUpdaterOverlay<SpotLight> attenuationUpdater;

    LightPositionUpdaterOverlay<SpotLight> positionUpdater;

    LightDirectionUpdaterOverlay<SpotLight> directionUpdater;

    SpotLightCutoffUpdaterOverlay cutoffUpdater;

};

} // namespace ape
