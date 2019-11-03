#pragma once

#include <Foundational/Signal/ScopedSignalConnection.hpp>

namespace ape
{

class Body;
class PointLight;

class BodyPointLightSynchronizer
{

public:

    BodyPointLightSynchronizer(Body const & source, PointLight & target);

private:

    auto registerPositionChangeEventHandler(Body const & source, PointLight & target)
        -> ScopedSignalConnection;

private:

    ScopedSignalConnection handlerConnection;

};

} // namespace ape