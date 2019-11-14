#pragma once

#include <Basix/Signal/ScopedSignalConnection.hpp>

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
        -> basix::ScopedSignalConnection;

    auto synchronize(Camera const & camera, SpotLight & light) const
        -> void;

private:

    basix::ScopedSignalConnection handlerConnection;

};

} // namespace ape
