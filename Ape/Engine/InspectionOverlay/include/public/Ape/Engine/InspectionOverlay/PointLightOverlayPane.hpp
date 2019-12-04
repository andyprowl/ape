#pragma once

#include <Ape/Engine/InspectionOverlay/LightAttenuationUpdaterOverlay.hpp>
#include <Ape/Engine/InspectionOverlay/LightColorUpdaterOverlay.hpp>
#include <Ape/Engine/InspectionOverlay/LightOnOffStatusUpdaterOverlay.hpp>
#include <Ape/Engine/InspectionOverlay/LightPositionUpdaterOverlay.hpp>
#include <Ape/Engine/InspectionOverlay/LightSystemOverlayPane.hpp>

#include <Ape/World/Scene/PointLight.hpp>

namespace ape
{

class PointLightOverlayPane : public LightSystemOverlayPane<PointLight>
{

public:

    using LightSystemOverlayPane::LightSystemOverlayPane;

private:

    // virtual (from LightSystemOverlyPane<PointLight>)
    auto updateSelectedLightProperties()
        -> void override;

    auto updateViewProperties(PointLight & light)
        -> void;

private:

    LightOnOffStatusUpdaterOverlay onOffStatusUpdater;

    LightColorUpdaterOverlay colorUpdater;

    LightAttenuationUpdaterOverlay<PointLight> attenuationUpdater;

    LightPositionUpdaterOverlay<PointLight> positionUpdater;

};

} // namespace ape
