#pragma once

#include <Ape/Pipeline.hpp>
#include <Ape/Signal.hpp>

#include <glm/mat4x4.hpp>

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