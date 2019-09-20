#pragma once

#include "Scene.hpp"

class SampleScene : public Scene
{

public:
    
    SampleScene() = default;

    std::vector<ModelInstance *> lamps;

    SpotLight * playerFlashlight = nullptr;

    ModelInstance * rotatingContainer = nullptr;

    ModelInstance * scalingContainer = nullptr;

};