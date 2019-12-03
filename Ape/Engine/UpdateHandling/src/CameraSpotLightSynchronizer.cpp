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

    light.setPlacement(newPosition, newDirection);
}

} // namespace ape