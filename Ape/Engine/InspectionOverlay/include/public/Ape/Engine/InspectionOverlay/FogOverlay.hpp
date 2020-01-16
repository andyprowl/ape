#pragma once

#include <Ape/Engine/InspectionOverlay/DirectionalLightOverlayPane.hpp>
#include <Ape/Engine/InspectionOverlay/PointLightOverlayPane.hpp>
#include <Ape/Engine/InspectionOverlay/SpotLightOverlayPane.hpp>

#include <Basix/Mathematics/Position.hpp>
#include <Basix/Mathematics/Size.hpp>

namespace ape
{

class ImGuiWindow;
class Fog;

class FogOverlay
{

public:

    FogOverlay(
        basix::Position<int> const & initialPosition,
        basix::Size<int> const & initialSize,
        Fog & fog);

    auto update()
        -> void;

private:

    auto makeWindow() const
        -> ImGuiWindow;

private:

    basix::Position<int> initialPosition;

    basix::Size<int> initialSize;

    Fog * fog;

};

} // namespace ape
