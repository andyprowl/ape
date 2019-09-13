#pragma once

#include "camera.hpp"
#include "light.hpp"
#include "widget.hpp"

#include <vector>

class World
{

public:

    World(std::vector<Widget> widgets, Light const & light, Camera const & camera)
        : widgets{std::move(widgets)}
        , light{light}
        , camera{camera}
    {
    }

public:

    std::vector<Widget> widgets;

    Light light;

    Camera camera;

};