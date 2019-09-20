#pragma once

#include "CameraSpotlightSynchronizer.hpp"
#include "Scene.hpp"

class SampleScene : public Scene
{

public:
    
    SampleScene() = default;

    std::vector<Body *> lamps;

    SpotLight * playerFlashlight = nullptr;

    Body * rotatingContainer = nullptr;

    Body * scalingContainer = nullptr;

    std::vector<CameraSpotlightSynchronizer> spotLightSynchronizers;

};