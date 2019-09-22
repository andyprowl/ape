#pragma once

#include <Ape/BodyPointLightSynchronizer.hpp>
#include <Ape/CameraSpotlightSynchronizer.hpp>
#include <Ape/Scene.hpp>

#include <vector>

class SampleScene : public Scene
{

public:
    
    SampleScene() = default;

    std::vector<Body *> lamps;

    SpotLight * playerFlashlight = nullptr;

    Body * rotatingContainer = nullptr;

    Body * scalingContainer = nullptr;

    std::vector<CameraSpotlightSynchronizer> spotLightSynchronizers;

    std::vector<BodyPointLightSynchronizer> bodyLightSynchronizers;

};