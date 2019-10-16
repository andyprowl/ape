#pragma once

#include <Foundational/Signal/ScopedSignalConnection.hpp>

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
        -> ScopedSignalConnection;

private:

    ScopedSignalConnection handlerConnection;

};

} // namespace ape
