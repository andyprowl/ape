#include <Ape/CameraSpotlightSynchronizer.hpp>

#include <Ape/Camera.hpp>
#include <Ape/SpotLight.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

CameraSpotlightSynchronizer::CameraSpotlightSynchronizer(Camera const & camera, SpotLight & light)
    : handlerConnection{registerViewChangeEventHandler(camera, light)}
{
}

auto CameraSpotlightSynchronizer::registerViewChangeEventHandler(
    Camera const & camera,
    SpotLight & light)
    -> ScopedSignalConnection
{
    return camera.onViewChanged.registerHandler([&camera, &light] (glm::mat4 const & /*view*/)
    {
        light.position = camera.getPosition();

        light.direction = camera.getDirection();
    });
}

} // namespace ape
