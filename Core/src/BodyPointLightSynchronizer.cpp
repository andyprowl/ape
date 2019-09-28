#include <Core/BodyPointLightSynchronizer.hpp>

#include <Core/Body.hpp>
#include <Core/PointLight.hpp>

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

} // namespace ape
