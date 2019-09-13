#pragma once

#include "camera.hpp"
#include "light.hpp"
#include "body.hpp"

#include <vector>

class World
{

public:

    World(std::vector<Body> bodies, Light const & light, Camera const & camera)
        : bodies{std::move(bodies)}
        , light{light}
        , camera{camera}
    {
    }

public:

    std::vector<Body> bodies;

    Light light;

    Camera camera;

};