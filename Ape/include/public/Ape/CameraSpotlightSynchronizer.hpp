#pragma once

#include <Ape/ScopedSignalConnection.hpp>

namespace ape
{

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

} // namespace ape
