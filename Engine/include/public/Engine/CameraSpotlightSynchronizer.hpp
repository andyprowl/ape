#pragma once

#include <Signal/ScopedSignalConnection.hpp>

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

    auto synchronize(Camera const & camera, SpotLight & light) const
        -> void;

private:

    ScopedSignalConnection handlerConnection;

};

} // namespace ape