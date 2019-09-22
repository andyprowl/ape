#pragma once

#include <Ape/CameraSystem.hpp>
#include <Ape/Lighting.hpp>
#include <Ape/Body.hpp>

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