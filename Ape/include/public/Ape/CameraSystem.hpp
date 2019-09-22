#pragma once

#include <Ape/Camera.hpp>

#include <vector>

class CameraSystem 
{

public:

    CameraSystem()
        : CameraSystem{{}, nullptr}
    {
    }

    CameraSystem(std::vector<Camera> cameras, Camera * const activeCamera)
        : cameras{std::move(cameras)}
        , activeCamera{activeCamera}
    {
    }

public:

    std::vector<Camera> cameras;

    Camera * activeCamera;

};