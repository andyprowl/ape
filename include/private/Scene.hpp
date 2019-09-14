#pragma once

#include "Body.hpp"
#include "Camera.hpp"
#include "Lighting.hpp"

#include <vector>

class Scene
{

public:

    Scene(std::vector<Body> bodies, Lighting lighting, Camera const & camera)
        : bodies{std::move(bodies)}
        , lighting{std::move(lighting)}
        , camera{camera}
    {
    }

public:

    std::vector<Body> bodies;

    Lighting lighting;

    Camera camera;

};