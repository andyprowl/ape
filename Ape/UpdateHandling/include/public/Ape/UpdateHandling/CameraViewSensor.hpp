#pragma once

#include <Ape/UpdateHandling/Pipeline.hpp>

#include <Basix/Signal/Signal.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class Camera;

class CameraViewSensor : public Sender<glm::mat4>
{

public:

    explicit CameraViewSensor(Camera & camera);

private:

    auto registerViewChangeEventHandler(Camera & camera)
        -> basix::ScopedSignalConnection;

private:

    basix::ScopedSignalConnection handlerConnection;

};

} // namespace ape
