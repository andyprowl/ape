#pragma once

#include "LightAttenuationUpdaterOverlay.hpp"
#include "LightColorUpdaterOverlay.hpp"
#include "LightOnOffStatusUpdaterOverlay.hpp"
#include "LightPositionUpdaterOverlay.hpp"
#include "LightSystemOverlayPane.hpp"

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
