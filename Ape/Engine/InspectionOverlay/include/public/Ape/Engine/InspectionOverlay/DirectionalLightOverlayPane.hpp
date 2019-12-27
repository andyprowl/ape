#pragma once

#include <Ape/Engine/InspectionOverlay/LightColorUpdaterOverlay.hpp>
#include <Ape/Engine/InspectionOverlay/LightDirectionUpdaterOverlay.hpp>
#include <Ape/Engine/InspectionOverlay/LightOnOffStatusUpdaterOverlay.hpp>
#include <Ape/Engine/InspectionOverlay/LightShadowCastingStatusUpdaterOverlay.hpp>
#include <Ape/Engine/InspectionOverlay/LightSystemOverlayPane.hpp>

#include <Ape/World/Scene/DirectionalLight.hpp>

namespace ape
{

class DirectionalLightOverlayPane : public LightSystemOverlayPane<DirectionalLight>
{

public:

    using LightSystemOverlayPane::LightSystemOverlayPane;

private:

    // virtual (from LightSystemOverlyPane<DirectionalLight>)
    auto updateSelectedLightProperties()
        -> void override;

    auto updateViewProperties(DirectionalLight & light)
        -> void;

private:

    LightOnOffStatusUpdaterOverlay onOffStatusUpdater;

    LightShadowCastingStatusUpdaterOverlay shadowCastingStatusUpdater;

    LightColorUpdaterOverlay colorUpdater;

    LightDirectionUpdaterOverlay<DirectionalLight> directionUpdater;

};

} // namespace ape
