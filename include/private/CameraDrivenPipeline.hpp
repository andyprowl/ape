#pragma once

#include "CameraViewSensor.hpp"
#include "SpotLightOrientator.hpp"

class CameraDrivenPipeline
{

public:

    CameraDrivenPipeline(Camera & camera, SpotLight & light)
        : sensor{camera}
        , lightOrientator{light}
    {
        sensor.connect(lightOrientator);
    }

private:

    CameraViewSensor sensor;

    SpotLightOrientator lightOrientator;

};