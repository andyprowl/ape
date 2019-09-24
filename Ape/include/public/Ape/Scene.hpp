#pragma once

#include <Ape/Camera.hpp>
#include <Ape/Lighting.hpp>
#include <Ape/Body.hpp>

#include <vector>

namespace ape
{

class Scene
{

public:

    Scene() = default;

    Scene(std::vector<Body> bodies, std::vector<Camera> cameras, Lighting lighting)
        : bodies{std::move(bodies)}
        , cameras{std::move(cameras)}
        , lighting{std::move(lighting)}
    {
    }

public:

    std::vector<Body> bodies;

    std::vector<Camera> cameras;

    Lighting lighting;

};

} // namespace ape
