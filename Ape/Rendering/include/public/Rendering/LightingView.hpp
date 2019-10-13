#pragma once

#include <Mathematics/Size.hpp>

#include <Signal/ScopedSignalConnection.hpp>

#include <glm/mat4x4.hpp>

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
        -> std::vector<glm::mat4> const &;

    auto getSpotView() const
        -> std::vector<glm::mat4> const &;

    auto getPointView() const
        -> std::vector<glm::mat4> const &;

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

    std::vector<glm::mat4> directionalView;

    std::vector<glm::mat4> spotView;

    std::vector<glm::mat4> pointView;

    std::vector<ScopedSignalConnection> lightChangeHandlerConnections;

};

} // namespace ape
