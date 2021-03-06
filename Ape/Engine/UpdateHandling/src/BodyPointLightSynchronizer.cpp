#include <Ape/Engine/UpdateHandling/BodyPointLightSynchronizer.hpp>

#include <Ape/World/Scene/Body.hpp>
#include <Ape/World/Scene/PointLight.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

BodyPointLightSynchronizer::BodyPointLightSynchronizer(Body const & source, PointLight & target)
    : handlerConnection{registerPositionChangeEventHandler(source, target)}
{
}

auto BodyPointLightSynchronizer::registerPositionChangeEventHandler(
    Body const & source,
    PointLight & target)
    -> basix::ScopedSignalConnection
{
    return source.onLocalTransformationChanged.registerHandler(
        [&source, &target] (auto const & part)
    {
        if (isRoot(part))
        {
            target.setPosition(getPosition(source));
        }
    });
}

} // namespace ape
