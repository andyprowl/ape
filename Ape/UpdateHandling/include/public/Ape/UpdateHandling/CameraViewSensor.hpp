#pragma once

#include <Ape/UpdateHandling/Pipeline.hpp>

#include <Foundational/Signal/Signal.hpp>

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
        -> ScopedSignalConnection;

private:

    ScopedSignalConnection handlerConnection;

};

} // namespace ape
