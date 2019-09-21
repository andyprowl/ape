#pragma once

#include "CameraSystem.hpp"
#include "Lighting.hpp"
#include "Body.hpp"

#include <vector>

class Scene
{

public:

    Scene() = default;

    Scene(std::vector<Body> bodies, CameraSystem cameraSystem, Lighting lighting)
        : bodies{std::move(bodies)}
        , cameraSystem{std::move(cameraSystem)}
        , lighting{std::move(lighting)}
    {
    }

public:

    std::vector<Body> bodies;

    CameraSystem cameraSystem;

    Lighting lighting;

};