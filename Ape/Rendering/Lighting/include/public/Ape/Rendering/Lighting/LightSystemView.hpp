#pragma once

#include <Ape/Rendering/Lighting/DirectionalLightView.hpp>
#include <Ape/Rendering/Lighting/PointLightView.hpp>
#include <Ape/Rendering/Lighting/SpotLightView.hpp>

#include <Ape/World/Scene/Camera.hpp>

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
        -> std::vector<SpotLightView> const &;

    auto getDirectionalView() const
        -> std::vector<DirectionalLightView> const &;

    auto setViewSize(basix::Size<int> const & newViewSize)
        -> void;

private:

    LightSystem const * lightSystem;

    basix::Size<int> viewSize;

    std::vector<PointLightView> pointView;

    std::vector<SpotLightView> spotView;

    std::vector<DirectionalLightView> directionalView;

};

} // namespace ape
