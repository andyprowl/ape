#pragma once

#include <Ape/CameraViewSensor.hpp>
#include <Ape/SpotLightOrientator.hpp>

namespace ape
{

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

} // namespace ape
