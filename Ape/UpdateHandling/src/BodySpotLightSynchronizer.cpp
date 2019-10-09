#include <UpdateHandling/BodySpotLightSynchronizer.hpp>

#include <Scene/Body.hpp>
#include <Scene/BodyPart.hpp>
#include <Scene/SpotLight.hpp>

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
    -> ScopedSignalConnection
{
    return source.onLocalTransformationChanged.registerHandler(
        [&source, &target] (auto const & part)
    {
        if (isRoot(part))
        {
            auto const & transformation = part.getGlobalTransformation();

            target.position = glm::vec3{transformation[3]};

            target.direction = -glm::vec3{transformation[0]};
        }
    });
}

} // namespace ape
