#pragma once

#include <Ape/Engine/InspectionOverlay/DirectionalLightOverlayPane.hpp>
#include <Ape/Engine/InspectionOverlay/PointLightOverlayPane.hpp>
#include <Ape/Engine/InspectionOverlay/SpotLightOverlayPane.hpp>

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
        basix::Position2d<int> const & initialPosition,
        basix::Size2d<int> const & initialSize,
        LightSystem & lightSystem);

    auto update()
        -> void;

private:

    auto makeWindow() const
        -> ImGuiWindow;

private:

    basix::Position2d<int> initialPosition;

    basix::Size2d<int> initialSize;

    LightSystem * lightSystem;

    SpotLightOverlayPane spotLightPane;

    PointLightOverlayPane pointLightPane;

    DirectionalLightOverlayPane directionalLightPane;

};

} // namespace ape
