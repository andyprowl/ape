#pragma once

#include <Basix/Signal/ScopedSignalConnection.hpp>

namespace ape
{

class Body;
class SpotLight;

class BodySpotLightSynchronizer
{

public:

    BodySpotLightSynchronizer(Body const & source, SpotLight & target);

private:

    auto registerTransformationChangeEventHandler(Body const & source, SpotLight & target)
        -> basix::ScopedSignalConnection;

private:

    basix::ScopedSignalConnection handlerConnection;

};

} // namespace ape
