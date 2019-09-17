#pragma once

#include "ScopedSignalConnection.hpp"

class Camera;
class SpotLight;

class CameraSpotlightSynchronizer
{

public:

    CameraSpotlightSynchronizer(Camera const & camera, SpotLight & light);

private:

    auto registerViewChangeEventHandler(Camera const & camera, SpotLight & light)
        -> ScopedSignalConnection;

private:

    ScopedSignalConnection handlerConnection;

};