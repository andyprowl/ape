#include <Ape/Engine/UpdateHandling/CameraViewSensor.hpp>

#include <Ape/World/Scene/Camera.hpp>

namespace ape
{

CameraViewSensor::CameraViewSensor(Camera & camera)
    : handlerConnection{registerViewChangeEventHandler(camera)}
{
}

auto CameraViewSensor::registerViewChangeEventHandler(Camera & camera)
    -> basix::ScopedSignalConnection
{
    return camera.onViewChanged.registerHandler([this] (glm::mat4 const & view)
    {
        send(view);
    });
}

} // namespace ape
