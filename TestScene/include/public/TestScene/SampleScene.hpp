#pragma once

#include <Core/BodyPointLightSynchronizer.hpp>
#include <Core/CameraSpotlightSynchronizer.hpp>
#include <Core/Scene.hpp>

#include <vector>

class SampleScene : public ape::Scene
{

public:
    
    SampleScene() = default;

    std::vector<ape::Body *> lamps;

    ape::SpotLight * playerFlashlight = nullptr;

    ape::Body * rotatingContainer = nullptr;

    ape::Body * scalingContainer = nullptr;

    ape::Body * dragon = nullptr;

    ape::Body * nanosuit = nullptr;

    ape::Body * castle = nullptr;

    std::vector<ape::CameraSpotlightSynchronizer> spotLightSynchronizers;

    std::vector<ape::BodyPointLightSynchronizer> bodyLightSynchronizers;

};
