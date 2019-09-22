#include <Ape/CameraViewSensor.hpp>

#include <Ape/Camera.hpp>

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
