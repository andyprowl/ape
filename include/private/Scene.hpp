#pragma once

#include "CameraSystem.hpp"
#include "Lighting.hpp"
#include "ModelInstance.hpp"

#include <vector>

class Scene
{

public:

    Scene() = default;

    Scene(std::vector<ModelInstance> bodies, CameraSystem cameraSystem, Lighting lighting)
        : bodies{std::move(bodies)}
        , cameraSystem{std::move(cameraSystem)}
        , lighting{std::move(lighting)}
    {
    }

public:

    std::vector<ModelInstance> bodies;

    CameraSystem cameraSystem;

    Lighting lighting;

};