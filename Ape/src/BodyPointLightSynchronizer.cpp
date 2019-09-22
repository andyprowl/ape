#include <Ape/BodyPointLightSynchronizer.hpp>

#include <Ape/Body.hpp>
#include <Ape/PointLight.hpp>

#include <glm/mat4x4.hpp>

BodyPointLightSynchronizer::BodyPointLightSynchronizer(Body const & source, PointLight & target)
    : handlerConnection{registerPositionChangeEventHandler(source, target)}
{
}

auto BodyPointLightSynchronizer::registerPositionChangeEventHandler(
    Body const & source,
    PointLight & target)
    -> ScopedSignalConnection
{
    return source.onLocalTransformationChanged.registerHandler(
        [&source, &target] (auto const & part)
    {
        if (isRoot(part))
        {
            target.position = getPosition(source);
        }        
    });
}
