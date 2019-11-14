#include <Ape/UpdateHandling/BodySpotLightSynchronizer.hpp>

#include <Ape/Scene/Body.hpp>
#include <Ape/Scene/BodyPart.hpp>
#include <Ape/Scene/SpotLight.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

BodySpotLightSynchronizer::BodySpotLightSynchronizer(Body const & source, SpotLight & target)
    : handlerConnection{registerTransformationChangeEventHandler(source, target)}
{
}

auto BodySpotLightSynchronizer::registerTransformationChangeEventHandler(
    Body const & source,
    SpotLight & target)
    -> basix::ScopedSignalConnection
{
    return source.onLocalTransformationChanged.registerHandler(
        [&source, &target] (auto const & part)
    {
        if (isRoot(part))
        {
            auto const & transformation = part.getGlobalTransformation();

            auto const newPosition = glm::vec3{transformation[3]};

            auto const newDirection = -glm::vec3{transformation[0]};

            target.setPlacement(newPosition, newDirection);
        }
    });
}

} // namespace ape
