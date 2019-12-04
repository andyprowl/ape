#pragma once

#include "LightColorUpdaterOverlay.hpp"
#include "LightDirectionUpdaterOverlay.hpp"
#include "LightOnOffStatusUpdaterOverlay.hpp"
#include "LightSystemOverlayPane.hpp"

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

    LightColorUpdaterOverlay colorUpdater;

    LightDirectionUpdaterOverlay<DirectionalLight> directionUpdater;

};

} // namespace ape
