#pragma once

#include <Basix/Signal/ScopedSignalConnection.hpp>

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
        -> basix::ScopedSignalConnection;

private:

    basix::ScopedSignalConnection handlerConnection;

};

} // namespace ape
