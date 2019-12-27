#pragma once

#include <Ape/Engine/InspectionOverlay/LightAttenuationUpdaterOverlay.hpp>
#include <Ape/Engine/InspectionOverlay/LightColorUpdaterOverlay.hpp>
#include <Ape/Engine/InspectionOverlay/LightDirectionUpdaterOverlay.hpp>
#include <Ape/Engine/InspectionOverlay/LightOnOffStatusUpdaterOverlay.hpp>
#include <Ape/Engine/InspectionOverlay/LightPositionUpdaterOverlay.hpp>
#include <Ape/Engine/InspectionOverlay/LightShadowCastingStatusUpdaterOverlay.hpp>
#include <Ape/Engine/InspectionOverlay/LightSystemOverlayPane.hpp>
#include <Ape/Engine/InspectionOverlay/SpotLightCutoffUpdaterOverlay.hpp>

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

    LightShadowCastingStatusUpdaterOverlay shadowCastingStatusUpdater;

    LightColorUpdaterOverlay colorUpdater;

    LightAttenuationUpdaterOverlay<SpotLight> attenuationUpdater;

    LightPositionUpdaterOverlay<SpotLight> positionUpdater;

    LightDirectionUpdaterOverlay<SpotLight> directionUpdater;

    SpotLightCutoffUpdaterOverlay cutoffUpdater;

};

} // namespace ape
