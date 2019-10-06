#include <UpdateHandling/CameraViewSensor.hpp>

#include <Scene/Camera.hpp>

namespace ape
{

CameraViewSensor::CameraViewSensor(Camera & camera)
    : handlerConnection{registerViewChangeEventHandler(camera)}
{
}

auto CameraViewSensor::registerViewChangeEventHandler(Camera & camera)
    -> ScopedSignalConnection
{
    return camera.onViewChanged.registerHandler([this] (glm::mat4 const & view)
    {
        send(view);
    });
}

} // namespace ape
