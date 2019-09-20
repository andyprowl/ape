#pragma once

#include "ScopedSignalConnection.hpp"

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
