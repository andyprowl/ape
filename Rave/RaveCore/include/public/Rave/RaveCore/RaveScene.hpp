#pragma once

#include <Ape/Scene/Scene.hpp>
#include <Ape/UpdateHandling/BodyPointLightSynchronizer.hpp>
#include <Ape/UpdateHandling/BodySpotLightSynchronizer.hpp>
#include <Ape/UpdateHandling/CameraSpotlightSynchronizer.hpp>

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
