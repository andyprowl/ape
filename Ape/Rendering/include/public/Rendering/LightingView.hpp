#pragma once

#include <Scene/Camera.hpp>

#include <Mathematics/Size.hpp>

#include <vector>

namespace ape
{

class Lighting;
class SpotLight;

class LightingView
{

public:

    LightingView(Lighting const & lighting, Size<int> const & viewSize);

    auto getDirectionalView() const
        -> std::vector<Camera> const &;

    auto getSpotView() const
        -> std::vector<Camera> const &;

    auto getPointView() const
        -> std::vector<Camera> const &;

    auto setViewSize(Size<int> const & newViewSize)
        -> void;

private:

    auto registerForLightChangeNotifications()
        -> std::vector<ScopedSignalConnection>;

    auto registerForLightChangeNotifications(SpotLight const & light)
        -> ScopedSignalConnection;

    auto udpateLightView(SpotLight const & light)
        -> void;

private:

    Lighting const * lighting;

    Size<int> viewSize;

    std::vector<Camera> directionalView;

    std::vector<Camera> spotView;

    std::vector<Camera> pointView;

    std::vector<ScopedSignalConnection> lightChangeHandlerConnections;

};

} // namespace ape
