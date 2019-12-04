#pragma once

#include "DirectionalLightOverlayPane.hpp"
#include "PointLightOverlayPane.hpp"
#include "SpotLightOverlayPane.hpp"

#include <Basix/Mathematics/Position.hpp>
#include <Basix/Mathematics/Size.hpp>

namespace ape
{

class ImGuiWindow;
class LightSystem;

class LightSystemOverlay
{


public:

    LightSystemOverlay(
        basix::Position<int> const & initialPosition,
        basix::Size<int> const & initialSize,
        LightSystem & lightSystem);

    auto update()
        -> void;

private:

    auto makeWindow() const
        -> ImGuiWindow;

private:

    basix::Position<int> initialPosition;

    basix::Size<int> initialSize;

    LightSystem * lightSystem;

    SpotLightOverlayPane spotLightPane;

    PointLightOverlayPane pointLightPane;

    DirectionalLightOverlayPane directionalLightPane;

};

} // namespace ape
