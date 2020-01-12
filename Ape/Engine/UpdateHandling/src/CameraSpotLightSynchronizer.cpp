#include <Ape/Engine/UpdateHandling/CameraSpotlightSynchronizer.hpp>

#include <Ape/World/Scene/Camera.hpp>
#include <Ape/World/Scene/SpotLight.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

CameraSpotlightSynchronizer::CameraSpotlightSynchronizer(Camera const & camera, SpotLight & light)
    : handlerConnection{registerViewChangeEventHandler(camera, light)}
{
    synchronize(camera, light);
}

auto CameraSpotlightSynchronizer::registerViewChangeEventHandler(
    Camera const & camera,
    SpotLight & light)
    -> basix::ScopedSignalConnection
{
    return camera.onViewChanged.registerHandler(
        [this, &camera, &light] (glm::mat4 const & /*view*/)
    {
        synchronize(camera, light);
    });
}

auto CameraSpotlightSynchronizer::synchronize(Camera const & camera, SpotLight & light) const
    -> void
{
    auto & view = camera.getView();

    auto const newPosition = view.getPosition();

    auto const newDirection = view.getDirection();

    // TODO: This is meant to be a temporary hack for getting a nice rendering result for a
    // headlight in the fog. Due to the way fog calculations work, having the headlight placed
    // slightly behind the viewer prevents singularity (when light and camera coincide) and also
    // makes fog around the viewer to be partly illuminated even outside the spotlight's cone.
    light.setPlacement(newPosition - newDirection * 0.05f, newDirection);
}

} // namespace ape
