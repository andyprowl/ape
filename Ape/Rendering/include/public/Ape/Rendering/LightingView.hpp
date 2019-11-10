#pragma once

#include <Ape/Rendering/PointLightView.hpp>

#include <Foundational/Mathematics/Size.hpp>
#include <Foundational/Signal/ScopedSignalConnection.hpp>

#include <glm/mat4x4.hpp>

#include <vector>

namespace ape
{

class DirectionalLight;
class Lighting;
class PointLight;
class SpotLight;

class LightingView
{

public:

    LightingView(Lighting const & lighting, Size<int> const & viewSize);
    
    auto getLighting() const
        -> Lighting const &;

    auto getPointView() const
        -> std::vector<PointLightView> const &;

    auto getSpotView() const
        -> std::vector<glm::mat4> const &;

    auto getDirectionalView() const
        -> std::vector<glm::mat4> const &;

    auto setViewSize(Size<int> const & newViewSize)
        -> void;

private:

    auto registerForPointLightChangeNotifications()
        -> std::vector<ScopedSignalConnection>;

    auto registerForSpotLightChangeNotifications()
        -> std::vector<ScopedSignalConnection>;

    auto registerForDirectionalLightChangeNotifications()
        -> std::vector<ScopedSignalConnection>;

    template<typename LightType>
    auto registerForLightChangeNotifications(LightType const & light)
        -> ScopedSignalConnection;

    auto udpateLightView(PointLight const & light)
        -> void;

    auto udpateLightView(SpotLight const & light)
        -> void;

    auto udpateLightView(DirectionalLight const & light)
        -> void;

private:

    Lighting const * lighting;

    Size<int> viewSize;

    std::vector<PointLightView> pointView;

    std::vector<glm::mat4> spotView;

    std::vector<glm::mat4> directionalView;

    std::vector<ScopedSignalConnection> pointLightChangeHandlerConnections;

    std::vector<ScopedSignalConnection> spotLightChangeHandlerConnections;

    std::vector<ScopedSignalConnection> directionalLightChangeHandlerConnections;

};

} // namespace ape
