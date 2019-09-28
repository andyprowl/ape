#pragma once

#include <Core/CameraViewSensor.hpp>
#include <Core/SpotLightOrientator.hpp>

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
