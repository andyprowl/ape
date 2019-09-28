#pragma once

#include <Core/Camera.hpp>
#include <Core/Lighting.hpp>
#include <Core/Body.hpp>

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
