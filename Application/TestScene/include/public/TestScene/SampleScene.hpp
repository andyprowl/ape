#pragma once

#include <UpdateHandling/BodyPointLightSynchronizer.hpp>
#include <UpdateHandling/BodySpotLightSynchronizer.hpp>
#include <UpdateHandling/CameraSpotlightSynchronizer.hpp>

#include <Scene/Scene.hpp>

#include <vector>

class SampleScene : public ape::Scene
{

public:
    
    SampleScene() = default;

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
