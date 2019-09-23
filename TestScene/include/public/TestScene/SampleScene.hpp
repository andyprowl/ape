#pragma once

#include <Ape/BodyPointLightSynchronizer.hpp>
#include <Ape/CameraSpotlightSynchronizer.hpp>
#include <Ape/Scene.hpp>

#include <vector>

class SampleScene : public ape::Scene
{

public:
    
    SampleScene() = default;

    std::vector<ape::Body *> lamps;

    ape::SpotLight * playerFlashlight = nullptr;

    ape::Body * rotatingContainer = nullptr;

    ape::Body * scalingContainer = nullptr;

    std::vector<ape::CameraSpotlightSynchronizer> spotLightSynchronizers;

    std::vector<ape::BodyPointLightSynchronizer> bodyLightSynchronizers;

};
