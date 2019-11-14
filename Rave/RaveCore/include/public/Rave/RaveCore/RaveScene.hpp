#pragma once

#include <Ape/Engine/UpdateHandling/BodyPointLightSynchronizer.hpp>
#include <Ape/Engine/UpdateHandling/BodySpotLightSynchronizer.hpp>
#include <Ape/Engine/UpdateHandling/CameraSpotlightSynchronizer.hpp>
#include <Ape/World/Scene/Scene.hpp>

#include <vector>

namespace rave
{

class RaveScene : public ape::Scene
{

public:
    
    RaveScene() = default;

    std::vector<ape::Body *> lamps;

    std::vector<ape::Body *> flashlights;

    ape::SpotLight * playerFlashlight = nullptr;

    ape::Body * rotatingContainer = nullptr;

    ape::Body * scalingContainer = nullptr;

    ape::Body * dragon = nullptr;

    ape::Body * nanosuit = nullptr;

    ape::Body * castle = nullptr;

    std::vector<ape::CameraSpotlightSynchronizer> cameraSpotLightSynchronizers;

    std::vector<ape::BodyPointLightSynchronizer> bodyPointLightSynchronizers;

    std::vector<ape::BodySpotLightSynchronizer> bodySpotLightSynchronizers;

};

} // namespace rave
