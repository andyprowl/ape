#pragma once

#include <Ape/Lighting/PointLightView.hpp>

#include <Basix/Mathematics/Size.hpp>
#include <Basix/Signal/ScopedSignalConnection.hpp>

#include <glm/mat4x4.hpp>

#include <vector>

namespace ape
{

class DirectionalLight;
class LightSystem;
class PointLight;
class SpotLight;

class LightSystemView
{

public:

    LightSystemView(LightSystem const & lightSystem, basix::Size<int> const & viewSize);
    
    auto getLighting() const
        -> LightSystem const &;

    auto getPointView() const
        -> std::vector<PointLightView> const &;

    auto getSpotView() const
        -> std::vector<glm::mat4> const &;

    auto getDirectionalView() const
        -> std::vector<glm::mat4> const &;

    auto setViewSize(basix::Size<int> const & newViewSize)
        -> void;

private:

    auto registerForPointLightChangeNotifications()
        -> std::vector<basix::ScopedSignalConnection>;

    auto registerForSpotLightChangeNotifications()
        -> std::vector<basix::ScopedSignalConnection>;

    auto registerForDirectionalLightChangeNotifications()
        -> std::vector<basix::ScopedSignalConnection>;

    template<typename LightType>
    auto registerForLightChangeNotifications(LightType const & light)
        -> basix::ScopedSignalConnection;

    auto udpateLightView(PointLight const & light)
        -> void;

    auto udpateLightView(SpotLight const & light)
        -> void;

    auto udpateLightView(DirectionalLight const & light)
        -> void;

private:

    LightSystem const * lightSystem;

    basix::Size<int> viewSize;

    std::vector<PointLightView> pointView;

    std::vector<glm::mat4> spotView;

    std::vector<glm::mat4> directionalView;

    std::vector<basix::ScopedSignalConnection> pointLightChangeHandlerConnections;

    std::vector<basix::ScopedSignalConnection> spotLightChangeHandlerConnections;

    std::vector<basix::ScopedSignalConnection> directionalLightChangeHandlerConnections;

};

} // namespace ape
