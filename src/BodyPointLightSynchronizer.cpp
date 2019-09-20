#include "BodyPointLightSynchronizer.hpp"

#include "Body.hpp"
#include "PointLight.hpp"

#include <glm/mat4x4.hpp>

/*
BodyPointLightSynchronizer::BodyPointLightSynchronizer(Body const & source, PointLight & target)
    : handlerConnection{registerPositionChangeEventHandler(source, target)}
{
}

auto BodyPointLightSynchronizer::registerPositionChangeEventHandler(
    Body const & source,
    PointLight & target)
    -> ScopedSignalConnection
{
    return source.onPositionChanged.registerHandler([&source, &target] (glm::vec3 const &)
    {
        target.position = getPosition(source);
    });
}
*/