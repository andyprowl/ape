#include <Engine/CameraSpotlightSynchronizer.hpp>

#include <Scene/Camera.hpp>
#include <Scene/SpotLight.hpp>

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
    -> ScopedSignalConnection
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
    light.position = camera.getPosition();

    light.direction = camera.getDirection();
}

} // namespace ape